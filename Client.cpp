// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Client.h"


// default constructor
Client::Client() {
   // default constructor
}


// contructor that recieves a socket
Client::Client(int socket) {
   sd = socket;   // set socket descriptor
   playGame();    // start the game for the client
}


// destructor
Client::~Client() {
   cout << "\n\nGoodbye..." << endl;
}


/**
 * @brief Asks the user to select an option based on the menu recieved from the server.
 * @return returns the selected option between 1-7
*/
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
   }

   sendMsg(ans);
   return ans;
}


/**
 * @brief Starts the game and directs the user to their desired menu selection
*/
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
            else {
               printBuffer();
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
   printBuffer();
   string result(buffer);
   if (result.substr(0, 7).compare("Welcome") == 0) {
      return true;
   }
   return false;
}


/**
 * @brief Creates a best 2 out of 3 environment for the client 
 * to play a match with another player on the server.
*/
void Client::bestOutOfThree() {
   while (true) {
      cout << "Waiting for opponent...\n" << endl;
      // message for each round
      printBuffer();

      // ask client to make a choice and sends the choice to the server
      sendMsg(makeChoice());  

      // player waits for result from server
      recvMsg(); 
      string result(buffer);
      
      // if message contains Exit then game is over
      if (result.substr(result.length()-4, result.length()).compare("Exit") == 0) {
         cout << result.substr(0, result.length()-4);
         break;
      }
      cout << result;
   }
   string endGame[11] = {
      "G", 
      "   a", 
      "      m", 
      "         e", 
      "             ", 
      "               o", 
      "                  v", 
      "                     e", 
      "                        r", 
      "\n"};
   for (int i = 0; i < 11; i++) {
      cout << endGame[i] << endl;
      sleep(1);
   }
}


/**
 * @brief Asks the user to make a choice between rock, paper, scissors
 * will keep asking until a correct choice has been made.
 * @return returns a copy of the choice to bestOutofThree 
*/
string Client::makeChoice() {
   string choice = "";
   int ans = -1;
   // Loop checking for valid input
   while (ans == -1) {
      cout << "Type in your option (rock, paper, scissors): ";
      cin >> choice;
      ans = convertAnswer(choice); // Converts string to int
   }
   return choice;
}


/**
 * @brief converts the input rock, paper, scissors to lower case and
 * compared the string to expected options to verify correct input.
 * @param input the provided string from the user
 * @return returns corresponding enum value or -1 if incorrect input
*/
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


/**
 * @brief converts a string of characters between A-Z to lowercase
 * @param input the provided string to convert to lowercase
*/
void Client::convertToLower(string &input) {
   for (int i = 0; i < input.length(); i++) {
      if (input[i] >= 'A' && input[i] <= 'Z') {
         input[i] = input[i] + 32;
      }
   }
}

void Client::printBuffer() {
   recvMsg();
   cout << buffer;
}

/**
 * @brief Sends a message through the clients socket descriptor
 * @param msg the message to be sent
*/
void Client::sendMsg(string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(sd, buffer, sizeof(buffer), 0);
}


/**
 * @brief Recieves a message from the socket descriptor into the buffer
*/
void Client::recvMsg() {
   memset(&buffer, 0, sizeof(buffer));
   recv(sd, buffer, sizeof(buffer), 0);
}