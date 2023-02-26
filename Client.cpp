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
      cout << "Type in your option (1 - 6): ";
      cin >> ans;
      if (isdigit(ans[0])) {
         if (stoi(ans) > 1 || stoi(ans) < 6) {
            break;
         }
      }
      ans = "0";
      cout << "Invalid option.\n";
   }

   sendMsg(ans);
   return ans;
}

void Client::playGame() {
   recvMsg(sd); // recieve welcome message
   cout << buffer; 

   // Direct client to selected menu option
   string option = "0";
   while (option.compare("6") != 0) {
      recvMsg(sd);   // recieve the menu message
      cout << buffer; 

      option = menuChoice();  // Gives user a menu to select from

      // handles the result from selected menu item

      // recieve the rules message
      if (option.compare("1") == 0) {
         recvMsg(sd);
         cout << buffer;
         sleep(3);
      }
      // display the scoreboard
      else if (option.compare("2") == 0) {}
      else if (option.compare("3") == 0) {}
      else if (option.compare("4") == 0) {}
      // begins a match best 2 out of 3
      else if (option.compare("5") == 0) {
         bestOutOfThree();
      }
   }
}

void Client::bestOutOfThree() {
   // best 2 out of 3 
   int round = 1;
   while (true) {
      // message for each round
      cout << "\nRound: " << round++ << endl;

      // player makes choice and send to server
      makeChoice();
      sendMsg(choice);

      // player waits for result from server
      recvMsg(sd); 
      string result(buffer);
       recvMsg(sd); 
       result += buffer;
      if (result.substr(result.length()-4, result.length()).compare("Exit") == 0) {
         cout << result.substr(0, result.length()-4);
         break;
      }
      cout << result;
   }
   char gameover[11] = {'G', 'a', 'm', 'e', ' ', 'O', 'v', 'e', 'r', '!', '\n'};
   for (int i = 0; i < 11; i++) {
      cout << gameover[i] << "\n";
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

void Client::sendMsg(string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(sd, buffer, sizeof(buffer), 0);
}

void Client::recvMsg(int sd) {
   memset(&buffer, 0, sizeof(buffer));
   recv(sd, buffer, sizeof(buffer), 0); 
}