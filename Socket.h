// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

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

class Socket {
   public:
      Socket();  
      ~Socket();
      void createServer();
      int createClient(const char* destinationAddr);
      int getClientSD();

   private:
      const char* PORT;                // port number to listen on
      struct addrinfo s_hints, *s_res, c_hints, *c_res;     // containers to store address info
      int serverSD, clientSD;          // Socket Descriptor
      int maxThreads;                  // Maximum threads
};

#endif