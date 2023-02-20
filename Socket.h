#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>         // memset 
#include <netdb.h>         // getaddrinfo
#include <sys/types.h>     // socket, bind 
#include <sys/socket.h>    // socket, bind, listen, inet_ntoa 
#include <unistd.h>        // read, write, close 
#include <netinet/in.h>    // htonl, htons, inet_ntoa 
#include <arpa/inet.h>     // inet_ntoa 
#include <netinet/tcp.h>   // SO_REUSEADDR 

using namespace std;

static int threadCount;                       // thread counter

class Socket {
   public:
      Socket();  
      ~Socket();
      void createServer();
      void createClient(const char* destinationAddr);

   private:
      const char* PORT;                // port number to listen on
      struct addrinfo hints, *res;     // containers to store address info
      int sd;                          // Socket Descriptor
      int maxPlayers;                  // Max players (threads)
};
#endif