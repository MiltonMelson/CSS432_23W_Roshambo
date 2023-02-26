#include "Server.h"
#include "Socket.h"

Socket::Socket() {
   PORT = (char*)"8080";
   maxThreads = 2; 
   threadCount = 0;
}

Socket::~Socket() {
   // close socket descriptor
   close(clientSD);
   close(serverSD);
   // free address info stored at res
   freeaddrinfo(res);
}

void* threadFunc(void *data) {
   Server game;
   game.startMenu(data);
   return data;
}

void Socket::createServer() {
   // setup address socket address info
   memset(&hints, 0, sizeof(hints));   // set block of memory to 0
   hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;        // fill in my IP for me 

   int status; 
   if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
      cerr << "Error: getaddrinfo" << endl;
      return;
   }

   // make a socket
   if ((serverSD = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
      cerr << "Error: Creating socket" << endl;
      return;
   }

   // loss the pesky "Address already in use" error message
   const int yes = 1;
   setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

   // bind the socket
   if (bind(serverSD, res->ai_addr, res->ai_addrlen) < 0) {
      cerr << "Error: failed to bind to socket" << endl;
      return;
   }

   cout << "Waiting for players..." << endl;

   // listen for N request
   if (listen(serverSD, maxThreads) < 0) {
      cerr << "Error: listening for players" << endl;
      return;
   } 

   // Create a thread for each accepted client
   struct sockaddr_storage clientAddr;
   socklen_t clientAddrSize = sizeof(clientAddr);
   pthread_t tid[maxThreads];
   while (threadCount < maxThreads) {
      int newSd = accept(serverSD, (struct sockaddr *)&clientAddr, &clientAddrSize);
      if (newSd < 0) {
         cerr << "Error: accepting player" << endl;
         return;
      }
      // create a new posix thread for each accepted player
      Player data(newSd, threadCount+1);
      int iret1 = pthread_create(&tid[threadCount], NULL, threadFunc, (void*)&data);
      threadCount++;
   }

   // Wait for child threads
   for (int i = 0; i < threadCount; i++) {
      pthread_join(tid[i], NULL);
   }
}

void Socket::createClient(const char* destinationAddr) {
   // setup address socket address info
   memset(&hints, 0, sizeof(hints));   // set block of memory to 0
   hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;        // fill in my IP for me 

   // get address info and store into *res
   int status;
   if ((status = getaddrinfo(destinationAddr, PORT, &hints, &res)) < 0) {
      cerr << "Error: getaddrinfo" << endl;
      return;
   }

   // create a socket
   if ((clientSD = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
      cerr << "Error: Creating socket" << endl;
      return;
   }

   // loss the pesky "Address already in use" error message 
   const int yes = 1;
   setsockopt(clientSD, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

   printf("Connecting to Server...\n\n");
   // connect to server through socket descriptor
   if (connect(clientSD, res->ai_addr, res->ai_addrlen) < 0) {
      cerr << "Error: Connecting to server" << endl;
      close(clientSD);
      return;
   }

}

int Socket::getClientSD() {
   return clientSD;
}