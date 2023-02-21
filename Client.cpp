#include "Client.h"

Client::Client() {}
Client::Client(int socket) {
   sd = socket;

   memset(&buffer, 0, sizeof(buffer));    // clear buffer
   recv(sd, buffer, sizeof(buffer), 0);   // recieve Welcome message and rules
   cout << buffer;                        // output message


   makeChoice();
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, choice.c_str());
   send(sd, buffer, sizeof(buffer), 0);

   memset(&buffer, 0, sizeof(buffer));
   recv(sd, buffer, sizeof(buffer), 0);   
   cout << buffer;       
}

Client::~Client() {
   cout << "\n\nGoodbye..." << endl;
}

// Asks client for there choice and returns int value of that choice
void Client::makeChoice() {
   int ans = -1;
   // Loop checking for valid input
   while (ans == -1) {
      cout << "Type in your option (rock, paper, scissors): ";
      getline(cin, choice);
      ans = convertAnswer(choice); // Converts string to int
   }
}

// converts string input from userChoice to verify correct input
int Client::convertAnswer(string input) {
   if (input == "Rock" || input == "rock" || input == "R" || input == "r" || input == "0") {
      return rock;
   }
   else if (input == "Paper" || input == "paper" || input == "P" || input == "p" || input == "1") {
      return paper;
   }
   else if (input == "Scissors" || input == "scissors" || input == "S" || input == "s" || input == "2") {
      return scissors;
   }
   cout << "\n\n!!! Sorry that is not a valid input !!!\n\n" << "Please enter: " << 
   "\n\tRock = (Rock, rock, R, or r)" << "\n\tPaper = (Paper, paper, P, or p)"
   << "\n\tScissors = (Scissors, scissors, S, or s)\n" << endl;
   return -1; // If no valid inputs, return -1 = false
}