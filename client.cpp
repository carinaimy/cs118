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

#include <sstream>
#include <fstream>


#include "CRC.h"

using namespace std;

#define BUFSIZE 1024
#define CRCSIZE 8

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


int main(int argc, char* argv[])
{
    
    // check argc
    if (argc != 4)
    {
        fprintf(stderr,"ERROR: Incorrect argc.\r\n");
        return 1;
    }
    // check port number
    if (!AllisNum(argv[2]))
    {
        fprintf(stderr,"ERROR: Incorrect port number.\r\n");
        return 1;
    }
    
    // get path and port
    int port = atoi(argv[2]);
    //cout<<"IP is:"<<argv[1]<<"port is:"<<port<<", path is:"<<argv[3]<<endl;
    
    // init crc64 table
    create_crc_table();
    
    // for file
    ifstream inFile(argv[3], ios::in|ios::binary); //二进制读方式打开
    if(!inFile) {
        perror("ERROR");
        return 1;
    }
    
    long l;
    inFile.seekg(0,ios::end);
    l = inFile.tellg();
    if(l> 100*1024*1024)
    {
        fprintf(stderr,"ERROR: The file is too large(over 100 MiB).\r\n");
        return 1;
    }
    
    // create a socket using TCP IP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);     // short, network byte order
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    
    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("ERROR:");
        return 2;
    }
    
    // send
    char buf[1024];
    inFile.seekg(0,ios::beg);
    while(1) {
        inFile.read((char *)buf, 1016);
        int size = inFile.gcount();
        cout << "read " << size << endl;
        if(size == 0)
            break;
        unsigned long long crc = htobe64(get_crc_code((unsigned char *)buf, size));
        buf[size] = (crc >> 56 ) & 0xFF;
        buf[size+1] = (crc >> 48 ) & 0xFF;
        buf[size+2] = (crc >> 40 ) & 0xFF;
        buf[size+3] = (crc >> 32 ) & 0xFF;
        buf[size+4] = (crc >> 24 ) & 0xFF;
        buf[size+5] = (crc >> 16 ) & 0xFF;
        buf[size+6] = (crc >> 8 ) & 0xFF;
        buf[size+7] = (crc) & 0xFF;
        size += 8;
        if (send(sockfd, buf, size, 0) == -1) {
            perror("ERROR");
            return 3;
        }
        //sleep(20);
    }
    
    cout<<"client read file over."<<endl;
    inFile.close();
    close(sockfd);
    
    return 0;
}
