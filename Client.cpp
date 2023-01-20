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
#include <cstdlib>        // atoi
#include <sys/time.h>     // gettimeofday
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

// Helper Functions
int convert(const string input);
int userChoice();
void showChoice(const RPS choice);
void welcomeMessage();
void displayRules();
void startGame();

int main(int argc, char* argv[]) {
   char* serverHost = argv[1];
   char* serverPort = argv[2];

}

/* Helper Functions */

void welcomeMessage() {
   cout << "-- Welcome to Roshambo! --" << endl;
   cout << "To view the rules type 'rules' or press enter to start the game." << endl;
   string input = "";
   cin >> input;
   do {
      displayRules();
      cout << "\nPress enter to start the game..." << endl;
      cin >> input;
   } while (input != "");
   startGame();
}

void displayRules() {
   cout << "Rules:\n" <<
   "This is a two player game. Each player will input (Rock, Paper, or Scissors).\n\n" <<
   "Acceptable format: \n(Rock: Rock, rock, R, or r)\n(Paper: Paper, paper, P, or p)\n(Scissors: Scissors, scissors, S, or s)\n\n"
   << "Game Logic: (Rock beats Scissors, Scissors beats Paper, Paper beats Rock)" << endl;
}

void startGame() {

}

int convert(const string input) {
   
   if (input == "Rock" || input == "rock" || input == "R" || input == "r" || input == "0") {
      return RPS::rock;
   }
   else if (input == "Paper" || input == "paper" || input == "P" || input == "p" || input == "1") {
      return RPS::paper;
   }
   else if (input == "Scissors" || input == "scissors" || input == "S" || input == "s" || input == "2") {
      return RPS::scissors;
   }
   cout << "Not a valid input!\n" << "Please enter:\n" << 
   "(Rock = Rock, rock, R, or r\n" << "Paper = Paper, paper, P, or p\n"
   << "Scissors = Scissors, scissors, S, or s" << endl;
   return -1; // If no valid inputs, return -1 = false
}

int userChoice() {
    string input;
    int choice = -1;

    // Loop checking for valid input
    while (choice == -1) {
        cout << "Type in your option (rock, paper, scissors): ";
        cin >> input;
        choice = convert(input); // Converts string to int
    }
    return choice;
}

void showChoice(const RPS choice) {
    switch (choice) {
        case rock: cout << "Rock"; break;
        case paper: cout << "Paper"; break;
        case scissors: cout << "Scissors"; break;
    }
}

