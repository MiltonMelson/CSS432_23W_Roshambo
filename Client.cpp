#include "Client.h"

Client::Client() {}

Client::Client(int socket) {
   sd = socket;
   playGame();
}

Client::~Client() {
   cout << "\n\nGoodbye..." << endl;
}

string Client::menuChoice() {
   string ans = "0";
   while (ans.compare("0") == 0) {
      cout << "Type in your option (1 - 7): ";
      cin >> ans;
      if (isdigit(ans[0])) {
         if (stoi(ans) >= 1 || stoi(ans) <= 7) {
            break;
         }
      }
      ans = "0";
      cout << "Invalid option.\n";
   }

   sendMsg(ans);
   return ans;
}

// Change
void Client::playGame() {
   printBuffer();

   string option = "0";
   while (option.compare("7") != 0) {
      printBuffer();
      option = menuChoice();
      int choice = atoi(option.c_str());
      switch(choice) {
         case 1:
            // View the rules
            printBuffer();
            break;
         case 2:
            // View the stats of an existing player
            displayStats();
            break;
         case 3:
            // View the leaderboard
            printBuffer();
            break;
         case 4:
            // Register as a new player
            if (reglogPlayer()) {
               bestOutOfThree();
            }
            break;
         case 5:
            // Log in as an existing player
            if(reglogPlayer()) {
               bestOutOfThree();
            }
            break;
         case 6:
            // Play as a guest
            bestOutOfThree();
            break;
         default:
            continue;
      }
   }
}

void Client::displayStats() {
   string ans;
   printBuffer();
   cin >> ans;
   sendMsg(ans);
   printBuffer();
}

bool Client::reglogPlayer() {
   string ans;
   printBuffer();
   cin >> ans;
   sendMsg(ans);
   string result(buffer);
   if (result.substr(0, 7).compare("Welcome") == 0) {
      return true;
   }
   return false;
}

void Client::bestOutOfThree() {
   // best 2 out of 3 
   int round = 1;
   while (true) {
      // message for each round
      printBuffer();

      // player makes choice and send to server
      makeChoice();
      sendMsg(choice);

      // player waits for result from server
      recvMsg(sd); 
      string result(buffer);
      
      // if message contains Exit then game is over
      if (result.substr(result.length()-4, result.length()).compare("Exit") == 0) {
         cout << result.substr(0, result.length()-4);
         break;
      }
      cout << result;
   }
   char endGame[11] = {'G', 'a', 'm', 'e', ' ', 'o', 'v', 'e', 'r', '\n'};
   for (int i = 0; i < 11; i++) {
      cout << endGame[i] << endl;
      sleep(1);
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
      if (input[i] >= 'A' && input[i] <= 'Z') {
         input[i] = input[i] + 32;
      }
   }
}

void Client::printBuffer() {
   recvMsg(sd);
   cout << buffer;
}

void Client::sendMsg(string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(sd, buffer, sizeof(buffer), 0);
}

void Client::recvMsg(int sd) {
   memset(&buffer, 0, sizeof(buffer));
   recv(sd, buffer, sizeof(buffer), 0); 
}