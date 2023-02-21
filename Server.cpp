#include "Server.h"
#include <sstream>

Server::Server() {
   maxPlayers = 2;
   answers[0] = "0";
   answers[1] = "0";
   scoreboard[0] = 0;
   scoreboard[1] = 0;
   playersReady = false;
}

Server::~Server() {
   cout << "\n\nGoodbye..." << endl;
}

void Server::startGame(void* info) {
   Player player = *(Player*)info;  // store info into player
   sendMessage(player, welcomeMessage() + displayRules());
   waitForPlayers(player);

   // best 2 out of 3
   int score = scoreboard[player.getID()-1];
   int enemyScore = scoreboard[(player.getID())%2];
   while (score < 2 && enemyScore < 2) {
      stringstream msg;
      waitForAnswers(player);
      msg << determineWinner(player);
      score = scoreboard[player.getID()-1];
      enemyScore = scoreboard[(player.getID())%2];
      msg << "\nYour score: " << score 
      << "\nEnemy score: " << enemyScore << "\n\n";
      if (score == 2 || enemyScore == 2) {
         if (score > enemyScore) {
            msg << "You Won the Match!\n\n";
         }
         else {
            msg << "You Lost the Match!\n\n";
         }
         msg << "Exit";
      }
      sendMessage(player, msg.str());
      answers[0] = "0";
      answers[1] = "0";
   }
   close(player.getSD());
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
   cout << "Player " << player.getID() << " has joined the game!" << endl;
   if (player.getID() == 2) {
      playersReady = true;
   }
   while (playersReady == false) {
      // wait for 2 players 
   }
}

void Server::waitForAnswers(Player player) {
   // wait for players to input answer
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
   player.setChoice(buffer);
   cout << "Player " << player.getID() << " Choice was: " << player.getChoice() << endl;
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[player.getID()-1] = player.getChoice();
   }
}

string Server::determineWinner(Player player) {
   string msg;
   
   // player 1 picks rock
   if (answers[0].compare("rock") == 0) {

      // player 2 picks paper
      if (answers[1].compare("paper") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            msg = "Paper covers Rock, You Lose!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "Paper covers Rock, You Win!";
         }
      }
      // player 2 picks scissors
      else if (answers[1].compare("scissors") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            scoreboard[0]++;
            msg = "Rock smashes Scissors, You Win!";
         }
         // player 2's thread
         else {
            msg = "Rock smashes Scissors, You Lose!";
         }
      }
      // opponent picks rock
      else {
         // Draw
         msg = "Draw!";
      }
   }
   // player 1 picks paper
   else if (answers[0].compare("paper") == 0) {

      // player 2 picks Scissors
      if (answers[1].compare("scissors") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            msg = "Scissors cuts paper, You Lose!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "Scissors cuts paper, You Win!";
         }
      }
      // player 2 picks Rock
      else if (answers[1].compare("rock") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            scoreboard[0]++;
            msg = "Paper covers Rock, You Win!";
         }
         // player 2's thread
         else {
            msg = "Paper covers Rock, You Lose!";
         }
      }
      // opponent picks Paper
      else {
         // Draw
         msg = "Draw!";
      }
   }
   // player 1 picks Scissors
   else {

      // player 2 picks Rock
      if (answers[1].compare("rock") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            msg = "Rock smashes Scissors, You Lose!";
         }
         // player 2's thread
         else {
            scoreboard[1]++;
            msg = "Rock smashes Scissors, You Win!";
         }
      }
      // player 2 picks Paper
      else if (answers[1].compare("paper") == 0) {

         // player 1's thread
         if (player.getID() == 1) {
            scoreboard[0]++;
            msg = "Scissors cuts Paper, You Win!";
         }
         // player 2's thread
         else {
            msg = "Scissors cuts Paper, You Lose!";
         }
      }
      // opponent picks Paper
      else {
         // Draw
         msg = "Draw!";
      }
   }
   return msg;
}

void Server::sendMessage(Player player, string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);
}