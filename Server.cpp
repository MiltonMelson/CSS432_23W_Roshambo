#include "Server.h"
#include <chrono>
#include <sstream>
#include <thread>


int roster[3];
int scoreboard[3];           
string answers[3]; 
string results[3];          

Server::Server() {
   maxPlayers = 2;         // current player capacity of the game
   for (int i = 0; i < maxPlayers+1; i++) {
      scoreboard[i] = 0;
      answers[i] = "0";
      roster[i] = 0;
      results[i] = "";
   }
}

Server::~Server() {
   cout << "Goodbye..." << endl;
}

// In progress
void Server::startMenu(void* info) {
   Player player = *(Player*)info;  // store info into player
   bool exit = false;
   int ans = 0;

   welcomeMessage(player);
   while (!exit) {
      menuMessage(player);
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
   close(player.getSD());  // close players socket descriptor
}

void Server::startGame(Player &player) {
   waitForPlayers(player); // waits for both players to join the game before 
   int score = 0;
   int enemyScore = 0;

   // best 2 out of 3
   while (score < 2 && enemyScore < 2) {
      waitForAnswers(player); // waits for both players to submit answers before proceeding 
      determineWinner(player);  // determine the winner and the resulting message for each player
      score = scoreboard[player.getID()];  // gets current players score
      enemyScore = scoreboard[getEnemyIndex(player)]; // gets the current players enemy score
   }
}

void Server::welcomeMessage(Player &player) {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n" << endl;
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::menuMessage(Player &player) {
   stringstream msg;
   msg << "Main Menu:\n" <<
   "1: View the rules\n" << 
   "2: View the leaderboard\n" << 
   "3: Register as a new player\n" <<
   "4: Log in as an existing player\n" << 
   "5: Play as a guest\n" <<
   "6: Exit Game\n" << endl;
   string temp(msg.str());
   sendMsg(player, temp);
}

void Server::displayRules(Player &player) {
   stringstream msg;
   msg << "\n~~~ Rules ~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n" << endl;
   string temp = msg.str();
   sendMsg(player, temp);
}

// In progress
string Server::displayBoard() {
   stringstream msg;
   string temp;
   return temp;
}

void Server::waitForPlayers(Player &player) {
   cout << "Player " << player.getID() << " has joined the game!" << endl;
   roster[player.getID()] = 1;
   while (roster[getEnemyIndex(player)] == 0) {
      // wait for opponent
   }
}

void Server::waitForAnswers(Player &player) {
   recvMsg(player); // wait for players to input answer
   player.setChoice(buffer);
   cout << "Player " << player.getID() << " Choice was: " << player.getChoice() << endl;
   answers[player.getID()] = player.getChoice();
   while (answers[getEnemyIndex(player)].compare("0") == 0) {
      // wait for opponent
   }
}

void Server::determineWinner(Player &player) {
   stringstream msg;
   string p1 = answers[player.getID()];
   string p2 = answers[getEnemyIndex(player)];

   // player 1 picks rock
   if (p1.compare("rock") == 0) {
      // player 2 picks paper
      if (p2.compare("paper") == 0) {
         msg << "Paper covers Rock, You Lost!";
      }
      // player 2 picks scissors
      else if (p2.compare("scissors") == 0) {
         msg << "Rock smashes Scissors, You Won!";
         scoreboard[player.getID()]++;
      }
      // opponent picks rock
      else {
         // Draw
         msg << "Draw!";
         player.setDraw();
      }
   }
   // player 1 picks paper
   else if (p1.compare("paper") == 0) {
      // player 2 picks Scissors
      if (p2.compare("scissors") == 0) {
         msg << "Scissors cuts Paper, You Lost!";
      }
      // player 2 picks Rock
      else if (p2.compare("rock") == 0) {
         msg << "Paper covers Rock, You Won!";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         player.setDraw();
      }
   }
   // player 1 picks Scissors
   else {
      // player 2 picks Rock
      if (p2.compare("rock") == 0) {
         msg << "Rock smashes Scissors, You Lost!";
      }
      // player 2 picks Paper
      else if (p2.compare("paper") == 0) {
         msg << "Scissors cuts Paper, You Won!";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         player.setDraw();
      }
   }
   msg << "\n" << endl;
   results[player.getID()] = msg.str();
   msg.clear();
   while (results[getEnemyIndex(player)].compare("") == 0) {
      // wait for opponent
      this_thread::sleep_for(chrono::microseconds(100));
   }
   this_thread::sleep_for(chrono::microseconds(100));
   sendMsg(player, results[player.getID()]);
   answers[player.getID()] = "0";

   int score = scoreboard[player.getID()];  // gets current players score
   int enemyScore = scoreboard[getEnemyIndex(player)]; // gets the current players enemy score

   msg << "\nYour score: " << score << 
   "\nEnemy score: " << enemyScore << "\n";

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
   msg << endl;
   sendMsg(player, msg.str());
   results[player.getID()] = "";
   while (results[getEnemyIndex(player)].compare("") != 0) {
      // wait for opponent
      this_thread::sleep_for(chrono::microseconds(100));
   }
}

int Server::getEnemyIndex(Player& player) {
   return player.getID()%2 == 0 ? player.getID()-1 : player.getID()+1;
}

void Server::sendMsg(Player &player, string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);
}

void Server::recvMsg(Player &player) {
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
}