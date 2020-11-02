CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=405346495

all: server client

server: 
	$(CXX) -o  server  $(CXXFLAGS) server.cpp CRC.cpp
	
client: 
	$(CXX) -o client $(CXXFLAGS) client.cpp CRC.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM server client *.tar.gz

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(405346495).tar.gz --exclude=./.vagrant . && mv /tmp/$(405346495).tar.gz .