#include "Server.h"
#include <sstream>

Server::Server() {
   maxPlayers = 2;
   answers[0] = "0";
   answers[1] = "0";
   playersReady = false;
}

Server::~Server() {
   cout << "\n\nGoodbye..." << endl;
}

void Server::startGame(void* info) {
   Player player = *(Player*)info;  // store info into player
   sendMessage(player, welcomeMessage() + displayRules());
   waitForPlayers(player);
   waitForAnswers(player);
   determineWinner(player);
   close(player.socketDescriptor);
}

string Server::welcomeMessage() {
   string msg = "\n---------------- Welcome to Roshambo ----------------\n\n";
   return msg;
}

string Server::displayRules() {
   stringstream msg;
   msg << "\nEach player will pick either rock, paper, or scissors.\n\n" <<
   "Acceptable Format:\n" << "\n\t(Rock: Rock, rock, R, or r)" <<
   "\n\t(Paper: Paper, paper, P, or p)" <<
   "\n\t(Scissors: Scissors, scissors, S, or s)\n\n" << 
   "How To Win: \n\n(Rock beats Scissors, Scissors beats Paper, Paper beats Rock)\n\n\n";
   string temp = msg.str();
   return temp;
}

void Server::waitForPlayers(Player player) {
   cout << "Player " << player.playerNumber << " has joined the game!" << endl;
   if (player.playerNumber == 2) {
      playersReady = true;
   }
   while (playersReady == false) {
      // wait for 2 players 
   }
}

void Server::waitForAnswers(Player player) {
   // wait for players to input answer
   memset(&buffer, 0, sizeof(buffer));
   recv(player.socketDescriptor, buffer, sizeof(buffer) , 0);
   player.choice = buffer;
   cout << "Player " << player.playerNumber << " Choice was: " << player.choice << endl;
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[player.playerNumber-1] = player.choice;
   }
}

void Server::determineWinner(Player player) {
   string msg;
   
   // player 1 picks rock
   if (answers[0].compare("rock") == 0) {

      // player 2 picks paper
      if (answers[1].compare("paper") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks scissors
      else if (answers[1].compare("scissors") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
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
   else if (answers[0].compare("paper") == 0) {

      // player 2 picks Scissors
      if (answers[1].compare("scissors") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks Rock
      else if (answers[1].compare("rock") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
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
      if (answers[1].compare("rock") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
            msg = "You Lose !!!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "You Win !!!";
         }
      }
      // player 2 picks Paper
      else if (answers[1].compare("paper") == 0) {

         // player 1's thread
         if (player.playerNumber == 1) {
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

   cout << "Player " << player.playerNumber << " picked: " << msg << endl;
   sendMessage(player, msg);
}

void Server::sendMessage(Player player, string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.socketDescriptor, buffer, sizeof(buffer), 0);
}