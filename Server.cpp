#include "Server.h"
#include <sstream>

Server::Server() {
   maxPlayers = 4;
   for (int i = 0; i < maxPlayers+1; i++) {
      scoreboard[i] = 0;
      answers[i] = "0";
      roster[i] = 0;
   }
}

Server::~Server() {
   cout << "\n\nGoodbye..." << endl;
}

// In progress
void Server::startMenu(void* info) {
   Player player = *(Player*)info;  // store info into player
   bool exit = false;
   int ans = 0;

   welcomeMessage(player); // send welcome message to player
   while (!exit) {
      menuMessage(player); // send menu message to player
      recvMsg(player);
      ans = atoi(buffer);
      switch(ans) {
         case 1:
            displayRules(player);
            break;
         case 2:
            sendMsg(player, displayBoard());
            break;
         case 3:
            //Register User
            break;
         case 4:
            // Sign in as User
            break;
         case 5:
            startGame(player);
            player.setGuest();
            break;
         case 6:
            exit = true;
            return;
         default:
            continue;
      }
   }
   close(player.getSD());  // close player socket descriptor
}

void Server::startGame(Player player) {
   waitForPlayers(player); // waits for 2 players to start a game

   int score = scoreboard[player.getID()];   // get players score
   int enemyScore = scoreboard[getEnemyIndex(player)];   // get players enemy score

   while (score < 2 && enemyScore < 2) {
      waitForAnswers(player);    // waits for both players to submit answers before proceeding 
      determineWinner(player);   // determine the winner and send message to player

      score = scoreboard[player.getID()];
      enemyScore = scoreboard[getEnemyIndex(player)];

      stringstream msg;
      msg << "\nYour score: " << score 
      << "\nEnemy score: " << enemyScore << "\n\n";

      // if either score is 2 then someone won
      if (score == 2 || enemyScore == 2) {
         // if its the current player then they won
         if (score > enemyScore) {
            msg << "You Won the Match!\n\n";
            if (!player.isGuest()) {
               player.setMatch();
            }
         }
         // if not then they lost
         else {
            msg << "You Lost the Match!\n\n";
         }
         if (!player.isGuest()) {
            player.setRound(scoreboard[0]);
         }
         msg << "Exit";
      }
      sendMsg(player, msg.str());   // sned a string message to the current player
   }
}

// In progress
string Server::displayBoard() {
   stringstream msg;
   string temp;
   return temp;
}

void Server::waitForPlayers(Player player) {
   cout << "Player " << player.getID() << " has joined the game!" << endl;
   roster[player.getID()] = 1;
   if (player.getID()%2 == 1) {
      while (roster[player.getID()+1] == 0) {
         // wait for opponent
      }
   }
}

void Server::waitForAnswers(Player player) {
   recvMsg(player); // wait for players to input answer
   player.setChoice(buffer);
   cout << "Player " << player.getID() << " Choice was: " << player.getChoice() << endl;
   answers[player.getID()] = player.getChoice();

   // if player number is odd
   if (player.getID()%2 == 1) {
      while (answers[player.getID()+1].compare("0") == 0) {
         // wait for opponent
      }
   } 
   // if player number is even
   else {
      while (answers[player.getID()-1].compare("0") == 0) {
         // wait for opponent
      }
   } 

   // player disconnects 
}

void Server::determineWinner(Player player) {
   string msg;
   msg = "\n\n";
   string player1 = answers[player.getID()];
   string player2 = answers[getEnemyIndex(player)];

   // player 1 picks rock
   if (player1.compare("rock") == 0) {
      // player 2 picks paper
      if (player2.compare("paper") == 0) {
         msg += "Paper covers Rock, You Lose!\n";
      }
      // player 2 picks scissors
      else if (player2.compare("scissors") == 0) {
         msg += "Rock smashes Scissors, You Win!\n";
         scoreboard[player.getID()]++;
      }
      else {
         // Draw
         msg += "Draw!";
         player.setDraw();
      }
   }
   // player 1 picks paper
   else if (player1.compare("paper") == 0) {
      // player 2 picks Scissors
      if (player2.compare("scissors") == 0) {
         msg += "Scissors cuts Paper, You Lose!\n";
      }
      // player 2 picks Rock
      else if (player2.compare("rock") == 0) {
         msg += "Paper covers Rock, You Win!\n";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg += "Draw!";
         player.setDraw();
      }
   }
   // player 1 picks Scissors
   else {
      // player 2 picks Rock
      if (player2.compare("rock") == 0) {
         msg += "Rock smashes Scissors, You Lose!\n";
      }
      // player 2 picks Paper
      else if (player2.compare("paper") == 0) {
         msg += "Scissors cuts Paper, You Win!\n";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg += "Draw!";
         player.setDraw();
      }
   }
   answers[player.getID()] = "0";
   answers[getEnemyIndex(player)] = "0";
   sendMsg(player, msg);
}

void Server::sendMsg(Player player, string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);
}

void Server::recvMsg(Player player) {
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
}

int Server::getEnemyIndex(Player player) {
   return player.getID()%2 == 1 ? player.getID()+1 : player.getID()-1;
}

void Server::welcomeMessage(Player player) {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::menuMessage(Player player) {
   stringstream msg;
   msg << "Main Menu:\n" << 
   "1: View the rules\n" << 
   "2: View the leaderboard\n" << 
   "3: Register as a new player\n" <<
   "4: Log in as an existing player\n" << 
   "5: Play as a guest\n" <<
   "6: Exit Game\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::displayRules(Player player) {
   stringstream msg;
   msg << "\n~~~~~~ Rules ~~~~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}