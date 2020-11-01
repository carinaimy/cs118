#include <string>
#include <thread>
#include <iostream>

#include "CRC.h"


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


using namespace std;

#define BUFSIZE 1024
#define CRCSIZE 8

int main(int argc, char* argv[]) 
{
    cerr << "server is not implemented yet" << endl;
    return 0;
}
