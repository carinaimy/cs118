#include <string>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include <fstream>
#include <sstream>
#include "CRC.h"

using namespace std;

#define BUFSIZE 1024
#define CRCSIZE 8

struct member
{
    int fd;
    int id;
};
string path;

// check port number
bool AllisNum (char * argv)
{
    int i = 0;
    while(argv[i] != '\0')
    {
        if(!(argv[i]>='0' && argv[i] <= '9'))
        {
            return false;
        }
        i++;
    }
    return true;
}


unsigned long long getCRC(unsigned char buf[]) {
    unsigned long long r, res = 0;
    r = buf[0];
    res |= r << 56;
    r = buf[1];
    res |= r << 48;
    r = buf[2];
    res |= r << 40;
    r = buf[3];
    res |= r << 32;
    r = buf[4];
    res |= r << 24;
    r = buf[5];
    res |= r << 16;
    r = buf[6];
    res |= r << 8;
    r = buf[7];
    res |= r;
    return res;
}


void* handleReq(void *argv)
{
    // get clientSockfd and connectionId
    struct member *temp = (struct member *)argv;
    int clientSockfd = temp->fd;
    int connectionId = temp->id;
    
    // output file
    ofstream outFile(path+"/"+to_string(connectionId)+".file", fstream::out);
    
    unsigned char fileBuf[BUFSIZE];
    int size;
    
    //for timeout
    struct timeval tv ={10, 0};
    
    while(1)
    {
        int ret = setsockopt(clientSockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));
        memset(fileBuf, 0, sizeof(fileBuf));
        size = recv(clientSockfd, fileBuf, BUFSIZE,0);
        if(size < 0) {
            outFile.write("ERROR: Timeout.", strlen("ERROR: Timeout."));
            break;
        }else if(size > 0){
            //cout<<"Recv:"<<size<<endl;
        }else{
            //cout<<"Recv end."<<endl;
            break;
        }
        
        unsigned long long crc = htobe64(get_crc_code((unsigned char *)fileBuf, size-CRCSIZE));
        unsigned long long recvCrc = getCRC(&fileBuf[size-CRCSIZE]);
        //printf("crc = %llx, recvCrc = %llx\n",crc,recvCrc);
        if(crc == recvCrc)
            outFile.write((const char*)fileBuf, size);
        else {
            outFile.write("ERROR: CRC error.", strlen("ERROR: CRC error."));
            break;
        }
    }
    cout<<"Connection close."<<endl;
    outFile.close();
    close(clientSockfd);
    return NULL;
}

int main(int argc, char* argv[])
{
    
    // check argc
    if (argc != 3)
    {
        perror("ERROR: Incorrect argc.");
        return 1;
    }
    // check port number
    if (!AllisNum(argv[1]))
    {
        perror("ERROR: Incorrect port number.");
        return 1;
    }
    
    // get path and port
    int port = atoi(argv[1]);
    path = argv[2];
    //cout<<"port is:"<<port<<", path is:"<<path<<endl;
    
    //init crc64 table
    create_crc_table();
    
    int connectionId = 1;
    // create a socket using TCP IP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // allow others to reuse the address
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }
    
    // bind address to socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);     // short, network byte order
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
    
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 2;
    }
    
    // set socket to listen status
    if (listen(sockfd, 1) == -1) {
        perror("listen");
        return 3;
    }
    
    // accept a new connection
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    cout<<"server is waiting for connection..."<<endl;
    while(1) {
        int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);
        
        if (clientSockfd == -1) {
            perror("accept");
            return 4;
        }
        cout<<"new connection, id: "<<connectionId<<endl;
        pthread_t ntid;
        struct member m;
        m.fd = clientSockfd;
        m.id = connectionId;
        pthread_create(&ntid, NULL, handleReq, (void *)&m);
        pthread_join(ntid, NULL);
        connectionId++;
    }
    return 0;
}
