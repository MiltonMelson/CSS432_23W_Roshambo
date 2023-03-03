#include "Server.h"
#include <chrono>
#include <sstream>
#include <thread>

int scoreboard[numOfPlayers];           
string answers[numOfPlayers]; 
string results[numOfPlayers]; 
bool threadLock;         

Server::Server() {
   threadLock = false;
   for (int i = 0; i < numOfPlayers; i++) {
      scoreboard[i] = 0;
      answers[i] = "0";
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
            scoreboard[player.getID()] = 0;
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
   cout << "Player " << player.getID() << " has joined the game!" << endl;
   int score = 0;
   int enemyScore = 0;
   int round = 1;
   // best 2 out of 3
   while (score < 2 && enemyScore < 2) {
      stringstream msg;
      msg << "\nRound " << round++ << endl;  // message for each round
      sendMsg(player, msg.str());
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
   string p1 = answers[player.getID()];
   string p2 = answers[getEnemyIndex(player)];
   
   stringstream msg;
   msg << "\n\n" << "~~~ " << p1 << " vs " << p2 << " ~~~\n\n";

   if (p1.compare("rock") == 0) {               // player 1 picks rock
      if (p2.compare("paper") == 0) {           // player 2 picks paper
         msg << "You Lost\n\nPaper covers Rock!";
      }
      else if (p2.compare("scissors") == 0) {   // player 2 picks scissors
         msg << "You Won\n\nRock smashes Scissors!";
         scoreboard[player.getID()]++;
      }
      else {                                    // player 2 picks rock
         msg << "Draw!";                        // Draw
         player.setDraw();
      }
   }
   else if (p1.compare("paper") == 0) {         // player 1 picks paper
      if (p2.compare("scissors") == 0) {        // player 2 picks Scissors
         msg << "You Lost\n\nScissors cuts Paper!";
      }
      else if (p2.compare("rock") == 0) {       // player 2 picks Rock
         msg << "You Won\n\nPaper covers Rock!";
         scoreboard[player.getID()]++;
      }
      else {                                    // player 2 picks Paper
         msg << "Draw!";                        // Draw
         player.setDraw();
      }
   } 
   else {                                       // player 1 picks Scissors
      if (p2.compare("rock") == 0) {            // player 2 picks Rock
         msg << "You Lost\n\nRock smashes Scissors!";
      }
      else if (p2.compare("paper") == 0) {      // player 2 picks Paper
         msg << "You Won\n\nScissors cuts Paper!";
         scoreboard[player.getID()]++;
      }
      else {                                    // opponent picks Paper
         msg << "Draw!";                        // Draw
         player.setDraw();
      }
   }
   this_thread::sleep_for(chrono::microseconds(100));
   int score = scoreboard[player.getID()];  // gets current players score
   int enemyScore = scoreboard[getEnemyIndex(player)]; // gets the current players enemy score
   msg << "\n\nYour score: " << score << 
   "\nEnemy score: " << enemyScore << "\n\n";

   // best 2 out of 3 for either score
   if (score == 2 || enemyScore == 2) {
      // if its the current players score is higher
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

   this_thread::sleep_for(chrono::microseconds(100));
   sendMsg(player, msg.str());
   answers[player.getID()] = "0";
}

int Server::getEnemyIndex(Player& player) {
   return player.getID()%2 == 0 ? player.getID()-1 : player.getID()+1;
}

void Server::sendMsg(Player &player, string msg) {
   this_thread::sleep_for(chrono::microseconds(player.getID()*100));
   while (threadLock) {
   // wait
      this_thread::sleep_for(chrono::microseconds(player.getID()*100));
   }
   threadLock = true;
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);
   threadLock = false;
}

void Server::recvMsg(Player &player) {
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
}