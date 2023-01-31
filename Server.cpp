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
int thread = 0;
int* scoreboard[2];
string answers[2];

struct thread_data {
   int sd;
   int tid;
   char* userChoice;
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
    while (thread < players) {
      int newSd = accept(sd, (struct sockaddr *)&clientAddr, &clientAddrSize);
      if (newSd < 0) {
         cerr << "Unable to accept client...";
         return 0;
      }
      // players connected to game
      cout << "Player " << thread+1 << " has joined the game!" << endl;
      // create a new posix thread for each accepted socket descriptor
      struct thread_data *data = new thread_data;
      data->sd = newSd;
      data->tid = thread;
      int iret1 = pthread_create(&tid[thread++], NULL, startGame, (void*)data);
    }
    for (int i = 0; i < thread; i++) {
      pthread_join(tid[i], NULL);
    }


   // close socket descriptor
   close(sd);
   freeaddrinfo(res);
   return 0;
}

void* startGame(void* data) {
   thread_data *ptr = (struct thread_data*)data;
   ptr->userChoice = new char[4000];
   answers[0] = "0";
   answers[1] = "0";
   char* msg;

   while (thread < players) {
      // wait for both players
   }
   cout << "Made it pass waiting for players" << endl;

   // reciever users choice
   recv(ptr->sd, ptr->userChoice, sizeof(ptr->userChoice), 0);   
   cout << "recievers choice: " << ptr->userChoice << endl;
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[ptr->tid] = ptr->userChoice;
   }

   cout << "passed waiting for answers" << endl;

   // player picks rock
   if (answers[0].compare("Rock") == 0) {

      // opponent picks paper
      if (answers[1].compare("Paper")) {
         msg = (char*)"You Lose!!";
         if (ptr->tid == 0) {
            scoreboard[1]++;
         }
         else {
            scoreboard[0]++;
         }
         send(ptr->sd, msg, sizeof(msg), 0);
      }
      // opponent picks scissors
      else if (answers[1].compare("Scissors") == 0) {
         msg = (char*)"You Win!!";
         if (ptr->tid == 0) {
            scoreboard[0]++;
         }
         else {
            scoreboard[1]++;
         }
         send(ptr->sd, msg, sizeof(msg), 0);
      }
      // opponent picks rock
      else {
         msg = (char*)"Game Draw";
         send(ptr->sd, msg, sizeof(msg), 0);
      }
   }

   // close client socket descriptor
   close(ptr->sd);
   return data;
}