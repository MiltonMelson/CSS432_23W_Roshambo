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
#include <sys/types.h>     // socket, bind 
#include <sys/socket.h>    // socket, bind, listen, inet_ntoa 
#include <netinet/in.h>    // htonl, htons, inet_ntoa 
#include <arpa/inet.h>     // inet_ntoa 
#include <netdb.h>         // gethostbyname 
#include <unistd.h>        // read, write, close 
#include <strings.h>       // bzero 
#include <netinet/tcp.h>   // SO_REUSEADDR 
#include <sys/uio.h>       // writev 
#include <cstring>         // memset 

using namespace std;

// Structure to hold thread data allowing access thread info during each match
struct thread_data {      
   int sd;
   int tid;
   char* userChoice;
};

const char* PORT;          // Port number to listen for players on
int players, thread;       // # of players, and thread counter
int* scoreboard[2];         // scoreboard to keep track of each players score
string answers[2];          // String array to store both players answers prior to determining each winner 


// helper functions
int createSocket();  
void* startGame(void* data);
void determineWinner(thread_data *ptr);

// Main function, Initializes global data and creates socket
int main(int argc, char *argv[]) {
   PORT = PORT = (char*)"8080";
   players = 2;
   thread = 0;
   return createSocket();
}

/**
 * @brief Creates the socket then listens for players. Once connected it will create a thread
 * for each player calling the start game function.
*/
int createSocket() {
   struct addrinfo hints, *res;        // containers to store address info
   memset(&hints, 0, sizeof(hints));   // set block of memory to 0
   hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
   hints.ai_flags = AI_PASSIVE;        // fill in my IP for me 

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

   // loss the pesky "Address already in use" error message
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

   // Create a thread for each accepted client
    struct sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    pthread_t tid[players];
    while (thread < players) {
      int newSd = accept(sd, (struct sockaddr *)&clientAddr, &clientAddrSize);
      if (newSd < 0) {
         cerr << "Unable to accept client...";
         return 0;
      }
      // create a new posix thread for each accepted socket descriptor
      struct thread_data *data = new thread_data;
      data->sd = newSd;
      data->tid = thread;
      int iret1 = pthread_create(&tid[thread++], NULL, startGame, (void*)data);
    }

    // Wait for all threads to finished prior to exiting game
    for (int i = 0; i < thread; i++) {
      pthread_join(tid[i], NULL);
    }

   // close socket descriptor
   close(sd);
   // free address info stored at res
   freeaddrinfo(res);
   return 0;
}


/**
 * @brief Starts the game for each player thread.
 * @param data The current thread_data pointer. Contains thread ID, socket descriptor, and players choice
*/
void* startGame(void* data) {
   thread_data *ptr = (struct thread_data*)data;
   ptr->userChoice = new char[10];
   answers[0] = "0";
   answers[1] = "0";

   // player # connected to game
   cout << "Player " << ptr->tid+1 << " has joined the game!" << endl;

   while (thread < players) {
      // wait for 2 players 
   }

   // wait for players to input answer
   recv(ptr->sd, ptr->userChoice, sizeof(ptr->userChoice), 0);   
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[ptr->tid] = ptr->userChoice;
   }

   // Decide winner and send message to clients
   determineWinner(ptr);

   // close client socket descriptor
   close(ptr->sd);
   return data;
}

/**
 * @brief Determines the winner of current match, following standard rock, paper, scissors rules.
 * Then sends the players the results of the match.
*/
void determineWinner(thread_data *ptr) {
   string msg;

   // player 1 picks rock
   if (answers[0].compare("Rock") == 0) {

      // player 2 picks paper
      if (answers[1].compare("Paper") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks scissors
      else if (answers[1].compare("Scissors") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            scoreboard[0]++;
            msg = "You Win !!!";
         }
         // player 2's thread
         else {
            msg = "You Lose !!!";
         }
      }
      // opponent picks rock
      else {
         // Draw
         msg = "Game Draw !!!";
      }
   }
   // player 1 picks paper
   else if (answers[0].compare("Paper") == 0) {

      // player 2 picks Scissors
      if (answers[1].compare("Scissors") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks Rock
      else if (answers[1].compare("Rock") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            scoreboard[0]++;
            msg = "You Win !!!";
         }
         // player 2's thread
         else {
            msg = "You Lose !!!";
         }
      }
      // opponent picks Paper
      else {
         // Draw
         msg = "Game Draw !!!";
      }
   }
   // player 1 picks Scissors
   else {

      // player 2 picks Rock
      if (answers[1].compare("Rock") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks Paper
      else if (answers[1].compare("Paper") == 0) {

         // player 1's thread
         if (ptr->tid == 0) {
            scoreboard[0]++;
            msg = "You Win !!!";
         }
         // player 2's thread
         else {
            msg = "You Lose !!!";
         }
      }
      // opponent picks Paper
      else {
         // Draw
         msg = "Game Draw !!!";
      }
   }

   // send appropriate message to current player (thread)
   send(ptr->sd, msg.c_str(), sizeof(msg), 0);
}
