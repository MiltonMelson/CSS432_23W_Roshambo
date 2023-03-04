// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Server.h"
#include "Socket.h"

/**
 * @brief Constructor, sets the port number and maximum number of threads
*/
Socket::Socket() {
   PORT = (char*)"8080";
   maxThreads = 100; 
}


/**
 * @brief Destructor, closes the client and servers socket descriptors
*/
Socket::~Socket() {
   // close socket descriptor
   close(clientSD);
   close(serverSD);
   // free address info stored at res
   freeaddrinfo(res);
}


/**
 * @brief Thread function to start the game for each thread created by the server.
 * @param data The data for each thread
 * @return the data in each thread
*/
void* threadFunc(void *data) {
   try {
      static Server game;     // Creates the game server only once
      game.startMenu(data);   // sends in each thread to the game
   }
   catch (exception& e) {
      cout << "An exception occured: " << e.what() << endl;
   }
   return data;            // returns the data of each thread
}


/**
 * @brief Creates the server side socket and listens for connections from the client
*/
void Socket::createServer() {
   // setup address socket address info
   memset(&hints, 0, sizeof(hints));   // set block of memory to 0
   hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;        // fill in my IP for me 

   // get the address info and store into res
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

   // lose the pesky "Address already in use" error message
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
   int threadCount = 0;
   while (threadCount < maxThreads) {
      int newSd = accept(serverSD, (struct sockaddr *)&clientAddr, &clientAddrSize);
      if (newSd < 0) {
         cerr << "Error: accepting player" << endl;
         return;
      }
      // create a new posix thread for each accepted player
      Player data(newSd);
      int iret1 = pthread_create(&tid[threadCount], NULL, threadFunc, (void*)&data);
      threadCount++;
   }

   // Wait for child threads created before main thread ends
   for (int i = 0; i < threadCount; i++) {
      pthread_join(tid[i], NULL);
   }
}


/**
 * @brief Creates the client side socket and attempts to connect to a server.
 * @param destinationAddr The server IP or Hostname the client is trying to connect to.
 * @return if 0 then connection failed, if 1 then connection successful.
*/
int Socket::createClient(const char* destinationAddr) {
   // setup address socket address info
   memset(&hints, 0, sizeof(hints));   // set block of memory to 0
   hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;        // fill in my IP for me 

   // get address info and store into *res
   int status;
   if ((status = getaddrinfo(destinationAddr, PORT, &hints, &res)) < 0) {
      cerr << "Error: getaddrinfo" << endl;
      return 0;
   }

   // create a socket
   if ((clientSD = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
      cerr << "Error: Creating socket" << endl;
      return 0;
   }

   // loss the pesky "Address already in use" error message 
   const int yes = 1;
   setsockopt(clientSD, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

   printf("Connecting to Server...\n\n");
   // connect to server through socket descriptor
   if (connect(clientSD, res->ai_addr, res->ai_addrlen) < 0) {
      cerr << "Error: Connecting to server" << endl;
      close(clientSD);
      return 0;
   }
   return 1;
}


/**
 * @brief Gets the current clients socketDescriptor
*/
int Socket::getClientSD() {
   return clientSD;
}