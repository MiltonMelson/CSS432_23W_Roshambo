#include "Roshambo.h"


Roshambo::Roshambo() {
   maxPlayers = 2;
   answers[0] = "0";
   answers[1] = "0";
}

Roshambo::~Roshambo() {
   // close client socket descriptor
   close(player->sd);
}

void Roshambo::startGame(void* info) {
   player = (struct playerInfo*)info;  // store info into player*
   player->choice = new char[10];

   cout << "Player " << player->ID+1 << " has joined the game!" << endl;

   while (player->ID+1 < maxPlayers) {
      // wait for 2 players 
   }
   cout << "Waiting for answers..." << endl;
   // wait for players to input answer
   recv(player->sd, player->choice, sizeof(player->choice), 0);   
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[player->ID] = player->choice;
   }

   // Decide winner and send message to clients
   determineWinner();
}

void Roshambo::enterLobby() {
   welcomeMessage();
   // send players choice to server
   RPS choice = (RPS)userChoice();
   char* ans = showChoice(choice);
   send(player->sd, ans, sizeof(ans), 0);
   // read results from match
   char buffer[256];
   recv(player->sd, buffer, sizeof(buffer), 0);
   cout << buffer << endl;
}

void Roshambo::determineWinner() {
   string msg;

   // player 1 picks rock
   if (answers[0].compare("Rock") == 0) {

      // player 2 picks paper
      if (answers[1].compare("Paper") == 0) {

         // player 1's thread
         if (player->ID == 0) {
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
         if (player->ID == 0) {
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
         if (player->ID == 0) {
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
         if (player->ID == 0) {
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
         if (player->ID == 0) {
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
         if (player->ID == 0) {
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
   send(player->sd, msg.c_str(), sizeof(msg), 0);
}

void Roshambo::welcomeMessage() {
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

void Roshambo::displayRules() {
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
int Roshambo::convertAnswer(string input) {
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

/** 
 * @brief Recieves input from the player and will keep checking until correct response is entered
*/
int Roshambo::userChoice() {
    string input;
    int choice = -1;
    // Loop checking for valid input
    while (choice == -1) {
        cout << "Type in your option (rock, paper, scissors): ";
        getline(cin, input);
        choice = convertAnswer(input); // Converts string to int
    }
    return choice;
}

/**
 * @brief Shows the users choice and also returns a string to send to server for comparisons 
 * @param choice Accepts a RPS const value
*/
char* Roshambo::showChoice(const RPS choice) {
   cout << "Your choice was: ";
   char* result;
    switch (choice) {
        case rock: 
         cout << "Rock"; 
         result = (char*)"Rock";
         break;
        case paper: 
         cout << "Paper"; 
         result = (char*)"Paper";
         break;
        case scissors: 
         cout << "Scissors"; 
         result = (char*)"Scissors";
         break;
    }
    cout << endl;
    return result;
}

