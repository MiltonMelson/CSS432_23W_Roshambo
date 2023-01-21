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
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>


using namespace std;

enum RPS { rock, paper, scissors };

// Helper Functions
int convert(string input);
int userChoice();
void showChoice(const RPS choice);
void welcomeMessage();
void displayRules();
void startGame();

int main(int argc, char* argv[]) {
   if (argc != 3) {
      cout << "Please enter {servers hostname, port#}" << endl;
      return 0;
   }
   char* serverIP = argv[1];     // servers ip address
   int port = atoi(argv[2]);     // servers port number

   // setup address information and connection tools
   struct hostent* host = gethostbyname(serverIP);
   struct sockaddr_in serverInfo;
   memset(&serverInfo, 0, sizeof(serverInfo));
   serverInfo.sin_family = AF_INET; 
   serverInfo.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
   serverInfo.sin_port = htons(port);

   // create socket
   int sd = socket(AF_INET, SOCK_STREAM, 0);
   cout << "Socket Created..." << endl;

   // Try to connect to socket descriptor 
   if ((connect(sd, (sockaddr*)&serverInfo, sizeof(serverInfo))) < 0) {
      cout << "Error trying to connect..." << endl;
      return -1; 
   }
   cout << "Connected to server..." << endl;


   // game logic
   startGame();
   

   // close sd
   close(sd);
   return 0;
}

/* ----- Helper Functions ----- */

void startGame() {
   welcomeMessage();
   RPS ans = (RPS)userChoice();
   showChoice(ans);
}

void welcomeMessage() {
   cout << "\n\t----- Welcome to Roshambo -----\n\n" << endl;
   cout << "To view the rules type 'rules' or press enter to start the game." << endl;
   string input = "";
   char* msg;
   getline(cin, input);
   strcpy(msg, input.c_str());
   if (strcmp(msg, "rules") == 0) {
      displayRules();
   }
}

void displayRules() {
   cout << "Rules:\n" <<
   "\tThis is a two player game. Each player will input (Rock, Paper, or Scissors).\n\n" <<
   "\tAcceptable format: \n\t\t(Rock: Rock, rock, R, or r)\n\t\t(Paper: Paper, paper, P, or p)\n\t\t(Scissors: Scissors, scissors, S, or s)\n\n"
   << "\tGame Logic: (Rock beats Scissors, Scissors beats Paper, Paper beats Rock)\n" << endl;
   cout << "Lets begin...\n" << endl;
}

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
   cout << "Sorry that is not a valid input!\n\n" << "Please enter: " << 
   "\n\tRock = Rock, rock, R, or r)" << "\n\tPaper = Paper, paper, P, or p"
   << "\n\tScissors = Scissors, scissors, S, or s" << endl;
   return -1; // If no valid inputs, return -1 = false
}

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

void showChoice(const RPS choice) {
    switch (choice) {
        case rock: 
         cout << "Rock\n"; 
         break;
        case paper: 
         cout << "Paper\n"; 
         break;
        case scissors: 
         cout << "Scissors\n"; 
         break;
    }
}

