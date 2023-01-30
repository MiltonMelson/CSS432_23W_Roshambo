/** 
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
 * 
 * Server.cpp
 * 
 * The server side will wait for two clients to connect before 
 * proceeding with the game logic of rock, paper, scissors.
 * Once the two clients have connected, the server waits for two 
 * responses from the clients which will be error checked on client side 
 * prior to sending their answer. The rules will follow basic rock, paper, scissors. 
 * Which is (Rock beats scissors, Scissors beats paper, Paper beats rock).
*/

#include <iostream>
#include <sys/types.h>    // socket, bind 
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa 
#include <netinet/in.h>   // htonl, htons, inet_ntoa 
#include <arpa/inet.h>    // inet_ntoa 
#include <netdb.h>        // gethostbyname 
#include <unistd.h>       // read, write, close 
#include <strings.h>      // bzero 
#include <netinet/tcp.h>  // SO_REUSEADDR 
#include <sys/uio.h>      // writev 
#include <cstring>        // memset 

using namespace std;
const char* PORT = (char*)"8080";
int players = 2;

struct thread_data {
   int sd;
   int userChoice;
};

// helper functions
void* startGame(void* data);

int main(int argc, char *argv[]) {

   // setup address info
   struct addrinfo hints, *res; 
   memset(&hints, 0, sizeof(hints)); 
   hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;     // fill in my IP for me 

   int status; 
   if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      return 0;
   }

   // make a socket
   int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
   if (sd < 0) {
      perror("Can't create server socket!");
      return 0;
   }

   /**
    * loss the pesky "Address already in use" error message
    * Set the SO_REUSEADDR option. (Note this option is useful to prompt OS to 
    * release the server port as soon as your server process is terminated.)
   */  
   const int yes = 1;
   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

   // bind the socket
   if (bind(sd, res->ai_addr, res->ai_addrlen) < 0) {
      perror("Can't bind to socket");
      close(sd);
      return 0;
   }
   
   cout << "Waiting for players..." << endl;
   // listen for N request
   if (listen(sd, players) < 0) {
      perror("Can't listen!");
      close(sd);
      return 0;
   } 

   // New player has joined the game create a thread
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    pthread_t tid[players];
    while (players > 0) {
      int newSd = accept(sd, (struct sockaddr *)&clientAddr, &clientAddrSize);
      if (newSd < 0) {
         cerr << "Unable to accept client...";
         return 0;
      }
      // players connected to game
      cout << "Player " << players << " has joined the game!" << endl;
      // create a new posix thread for each accepted socket descriptor
      struct thread_data *data = new thread_data;
      data->sd = newSd;
      int iret1 = pthread_create(&tid[players--], NULL, startGame, (void*)data);
    }
    players = 1;
    while (players <= 2) {
      pthread_join(tid[players++], NULL);
    }


   // close socket descriptor
   close(sd);
   freeaddrinfo(res);
   return 0;
}

void* startGame(void* data) {
   thread_data *ptr = (struct thread_data*)data;

   return data;
}