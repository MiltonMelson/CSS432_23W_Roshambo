// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Client.h"

Client::Client() {
   // default constructor
}


Client::Client(int socket) {
   sd = socket;   // set socket descriptor
   playGame();    // start the game for the client
}


Client::~Client() {
   cout << "\n\nGoodbye..." << endl;
}


/**
 * @brief Asks the user to select an option based on the menu recieved from the server.
 * @return returns the selected option between 1-6
*/
int Client::menuChoice() {
   int ans = 0;
   while (ans == 0) {
      cout << "Type in your option (1 - 6): ";
      cin >> ans;
      if (ans > 1 || ans < 6) {
         break;
      }
      ans = 0;
      cout << "Invalid option.\n";
   }
   sendMsg(to_string(ans));
   return ans;
}


/**
 * @brief Starts the game and directs the user to their desired menu selection
*/
void Client::playGame() {
   recvMsg();           // receives welcome message
   cout << buffer;      // output message

   while (true) {
      recvMsg();        // recieves menu message
      cout << buffer;
      int option = menuChoice();

      switch (option) {
         case 1:        // recieve rules message
            recvMsg();   
            cout << buffer;
            sleep(5);
            break;  
         case 2:
         case 3:
         case 4:
         case 5:        // play a round of rps
            bestOutOfThree();
            break;
         case 6:        // exit the game
            return;
      }
   }
}


/**
 * @brief Creates a best 2 out of 3 environment for the client 
 * to play a match with another player on the server.
*/
void Client::bestOutOfThree() {
   while (true) {
      // message for each round #
      recvMsg();    
      cout << buffer;

      // ask client to make a choice and sends the choice to the server
      sendMsg(makeChoice());  

      // player waits for result from server
      recvMsg(); 
      string result(buffer);
      
      // if message contains Exit then game is over
      if (result.substr(result.length()-4, result.length()).compare("Exit") == 0) {
         cout << result.substr(0, result.length()-4);
         break;
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