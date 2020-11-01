# Makefile - makefile to build the base system

# Make environment
CC=g++
LIBS=-lpthread


server :
	$(CC) CRC.cpp Server.cpp -o server $(LIBS)
client:
	$(CC) CRC.cpp Client.cpp -o client

	
