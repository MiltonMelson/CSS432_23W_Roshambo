/**
 * Created by: Oliver Jeremiah E Fernandez and Milton Melson
 * 
 * Client.cpp
 * 
 * The client side will establish a connecton to the server. 
 * Once connected it will display the rules of the game and 
 * the logic required to verify the correct input before sending to the server.
 * Once both players (Clients) have submitted their answer the server will reply with
 * a message stating if the player won, loss, or was a draw. 
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

enum RPS { rock, paper, scissors };

int sd;        // socket descriptor

// Helper Functions
int convert(string input);
int userChoice();
char* showChoice(const RPS choice);
void welcomeMessage();
void displayRules();
void startGame();
int connectSocket(char* serverIP, char* port);


int main(int argc, char* argv[]) {
   if (argc != 3) {
      cout << "Please enter {servers hostname, port#}" << endl;
      return 0;
   }
   // create socket and start game
   connectSocket(argv[1], argv[2]);
   return 0;
}

/* ----- Helper Functions ----- */

/**
 * @brief Creates a TCP connection between client and server
 * @param serverIP The provided IP address of the server
 * @param port The default port address being used
*/
int connectSocket(char* serverIP, char* port) {
   // Setup the addressinfo containers
   struct addrinfo hints, *res;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

   // get address info and store into *res
   int status;
   if ((status = getaddrinfo(serverIP, port, &hints, &res)) < 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      return -1;
   }

   // create a socket
   if ((sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
      perror("Error: Could not create socket");
      return -1;
   }
   /**
    * loss the pesky "Address already in use" error message
    * Set the SO_REUSEADDR option. (Note this option is useful to prompt OS to 
    * release the server port as soon as your server process is terminated.)
   */  
   const int yes = 1;
   setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));

   // connect to server through socket descriptor
   if (connect(sd, res->ai_addr, res->ai_addrlen) < 0) {
      perror("Error");
      close(sd);
      return -1;
   }
   printf("Connecting to Server...\n\n");

   // Starts the game
   startGame();
   
   // free address info
   freeaddrinfo(res);
   return 0;
}

/**
 * @brief Starts the game and communicates with game server
*/
void startGame() {
   welcomeMessage();

   // send players choice to server
   RPS choice = (RPS)userChoice();
   char* ans = (char*)showChoice(choice);
   send(sd, ans, sizeof(ans), 0);

   // read results from match
   char buffer[256];
   recv(sd, buffer, sizeof(buffer), 0);
   cout << buffer << endl;
   close(sd);
}

/**
 * @brief Welcome message and ask player if they want to read the rules.
*/
void welcomeMessage() {
   cout << "\n\n---------------- Welcome to Roshambo ----------------\n\n" << endl;
   cout << "To view the rules type 'rules' or press 'Enter' to start the game." << endl;
   string input;
   getline(cin, input);

   // convert string to lowercase for different case inputs
   for (int i = 0; i < input.length(); i++) {
      input[i] = tolower(input[i]);
   }
   if (input.compare("rules") == 0) {
      displayRules();
   }
   cout << "\n\n---------------- LET THE MATCH BEGIN -----------------\n\n" << endl;
}

/**
 * @brief Simple message explaining the rules of the game to the players
 */
void displayRules() {
   cout << "\n************************************************************" << endl;
   cout << "\nThis is a two player game. Each player will pick either rock, paper, or scissors.\n" << endl;
   sleep(4);
   cout << "Acceptable format:" << 
   "\n\t(Rock: Rock, rock, R, or r)" <<
   "\n\t(Paper: Paper, paper, P, or p)" <<
   "\n\t(Scissors: Scissors, scissors, S, or s)\n\n" << 
   "Game Logic: (Rock beats Scissors, Scissors beats Paper, Paper beats Rock)\n" << endl;
   cout << "************************************************************" << endl;
   sleep(8);
}

/** 
 * @brief Converts users choice to int 
 * @param input Can be a variation of rock, paper, scissors, or enum equivalent 
*/
int convert(string input) {
   if (input == "Rock" || input == "rock" || input == "R" || input == "r" || input == "0") {
      return RPS::rock;
   }
   else if (input == "Paper" || input == "paper" || input == "P" || input == "p" || input == "1") {
      return RPS::paper;
   }
   else if (input == "Scissors" || input == "scissors" || input == "S" || input == "s" || input == "2") {
      return RPS::scissors;
   }
   cout << "\n\n!!! Sorry that is not a valid input !!!\n\n" << "Please enter: " << 
   "\n\tRock = (Rock, rock, R, or r)" << "\n\tPaper = (Paper, paper, P, or p)"
   << "\n\tScissors = (Scissors, scissors, S, or s)\n" << endl;
   return -1; // If no valid inputs, return -1 = false
}

/** 
 * @brief Recieves input from the player and will keep checking until correct response is entered
*/
int userChoice() {
    string input;
    int choice = -1;
    // Loop checking for valid input
    while (choice == -1) {
        cout << "Type in your option (rock, paper, scissors): ";
        getline(cin, input);
        choice = convert(input); // Converts string to int
    }
    return choice;
}


/**
 * @brief Shows the users choice and also returns a string to send to server for comparisons 
 * @param choice Accepts a RPS const value
*/
char* showChoice(const RPS choice) {
   cout << "Your choice was: ";
   char* res;
    switch (choice) {
        case rock: 
         cout << "Rock"; 
         res = (char*)"Rock";
         break;
        case paper: 
         cout << "Paper"; 
         res = (char*)"Paper";
         break;
        case scissors: 
         cout << "Scissors"; 
         res = (char*)"Scissors";
         break;
    }
    cout << endl;
    return res;
}

