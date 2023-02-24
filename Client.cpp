#include "Client.h"

Client::Client() {}

Client::Client(int socket) {
   sd = socket;
   playGame();
}

Client::~Client() {
   cout << "\n\nGoodbye..." << endl;
}

void Client::playGame() {
   memset(&buffer, 0, sizeof(buffer));    // clear buffer
   recv(sd, buffer, sizeof(buffer), 0);   // recieve Welcome message and rules
   cout << buffer;                        // output message

   // best 2 out of 3 
   int round = 1;
   while (true) {
      // message for each round
      cout << "Round: " << round++ << endl;

      // player makes choice and send to server
      makeChoice();
      memset(&buffer, 0, sizeof(buffer));
      strcpy(buffer, choice.c_str());
      send(sd, buffer, sizeof(buffer), 0);

      // player waits for result from server
      memset(&buffer, 0, sizeof(buffer));
      recv(sd, buffer, sizeof(buffer), 0);   
      string result(buffer);
      if (result.substr(result.length()-4, result.length()).compare("Exit") == 0) {
         cout << result.substr(0, result.length()-4);
         return;
      }
      cout << result;
   }
}

// Asks client for there choice and returns int value of that choice
void Client::makeChoice() {
   int ans = -1;
   // Loop checking for valid input
   while (ans == -1) {
      cout << "Type in your option (rock, paper, scissors): ";
      cin >> choice;
      ans = convertAnswer(choice); // Converts string to int
   }
}

// converts string input from userChoice to verify correct input
int Client::convertAnswer(string &input) {
   convertToLower(input);
   if (input.compare("rock") == 0) {
      return 0;
   }
   else if (input.compare("paper") == 0) {
      return 1;
   }
   else if (input.compare("scissors") == 0) {
      return 2;
   }
   return -1;
}

void Client::convertToLower(string &input) {
   for (int i = 0; i < input.length(); i++) {
      if (input[i] > 'A' && input[i] < 'Z') {
         input[i] = input[i]+32;
      }
   }
}