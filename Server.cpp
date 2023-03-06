#include "Server.h"
#include <chrono>
#include <sstream>
#include <thread>


int roster[numOfPlayers];
int scoreboard[numOfPlayers];           
string answers[numOfPlayers]; 
string results[numOfPlayers]; 
bool threadLock;         

Server::Server() {
   threadLock = false;
   for (int i = 0; i < numOfPlayers; i++) {
      scoreboard[i] = 0;
      roster[i] = 0;
      answers[i] = "0";
      results[i] = "";
   }
}

Server::~Server() {
   cout << "Goodbye..." << endl;
}

void Server::startMenu(void* info) {
   Data data("textfile.txt");
   Player player = *(Player*)info;  // store info into player
   bool exit = false;
   int ans = 0;

   welcomeMessage(player);
   while (!exit) {
      menuMessage(player);
      recvMsg(player);
      ans = atoi(buffer);
      int code;
      switch(ans) {
         case 1:
            code = 1;
            displayRules(player);
            break;
         case 2:
            code = displayStat(player, data);
            break;
         case 3:
            code = displayBoard(player, data);
            break;
         case 4:
            code = regPlayer(player, data);
            if (code == 1) {
               startGame(player);
               scoreboard[player.getID()] = 0;
            };
            break;
         case 5:
            code = logPlayer(player, data);
            if (code == 1) {
               startGame(player);
               scoreboard[player.getID()] = 0;
            };
            break;
         case 6:
            code = 1;
            startGame(player);
            player.setGuest();
            scoreboard[player.getID()] = 0;
            break;
         case 7:
            code = 1;
            roster[player.getID()] = 0;
            exit = true;
            break;
         default:
            continue;
      }
      if (code != 1) {
         sendMsg(player, displayErr(code));
      }  
   }   
   close(player.getSD());  // close players socket descriptor
}

void Server::startGame(Player &player) {
   waitForPlayers(player); // waits for both players to join the game before 
   int score = 0;
   int enemyScore = 0;
   int round = 1;

   while (score < 2 && enemyScore < 2) {
      stringstream msg;
      msg << "\nRound " << round++ << endl;  // message for each round
      sendMsg(player, msg.str());

      score = scoreboard[player.getID()];
      enemyScore = scoreboard[getEnemyIndex(player)];

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
   "2: View the stats of an existing player\n" <<
   "3: View the leaderboard\n" << 
   "4: Register as a new player\n" <<
   "5: Log in as an existing player\n" << 
   "6: Play as a guest\n" <<
   "7: Exit Game\n" << endl;
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

int Server::displayStat(Player &player, Data &data) {
   string ans;

   sendMsg(player, "Type the name for the player's stats you want to view: ");
   recvMsg(player);

   int resp = data.getStats(ans, buffer);
   if (resp == 1) {
      sendMsg(player, ans);
   }
   return resp;
}

int Server::displayBoard(Player &player, Data &data) {
   string ans = "~~~ Roshambo Leaderboard! ~~~\n";
   int resp = data.getBoard(ans);
   if (resp == 1) {
      sendMsg(player, ans);
   }
   return resp;
}

int Server::regPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Type in your username (alphanumeric only, max 20 chars.): ");
   recvMsg(player);

   int resp = data.regUser(name);
   if (resp == 1) {
      sendMsg(player, "Welcome to Roshambo, " + name + "!");
      player.setName(name.c_str());
   }

   return resp;
}

int Server::logPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Username: ");
   recvMsg(player);

   int resp = data.logUser(name);
   if (resp == 1) {
      sendMsg(player, "Welcome back, " + name + "!");
   }

   return resp;
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
   msg << "\n\n" << "~~~ " << p1 << " vs " << p2 << " ~~~\n\n";
   // player 1 picks rock
   if (p1.compare("rock") == 0) {
      // player 2 picks paper
      if (p2.compare("paper") == 0) {
         msg << "You Lost\n\nPaper covers Rock!";
      }
      // player 2 picks scissors
      else if (p2.compare("scissors") == 0) {
         msg << "You Won\n\nRock smashes Scissors!";
         scoreboard[player.getID()]++;
      }
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }
   // player 1 picks paper
   else if (p1.compare("paper") == 0) {
      // player 2 picks Scissors
      if (p2.compare("scissors") == 0) {
         msg << "You Lost\n\nScissors cuts Paper!";
      }
      // player 2 picks Rock
      else if (p2.compare("rock") == 0) {
         msg << "You Won\n\nPaper covers Rock!";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }
   // player 1 picks Scissors
   else {
      // player 2 picks Rock
      if (p2.compare("rock") == 0) {
         msg << "You Lost\n\nRock smashes Scissors!";
      }
      // player 2 picks Paper
      else if (p2.compare("paper") == 0) {
         msg << "You Won\n\nScissors cuts Paper!";
         scoreboard[player.getID()]++;
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }
   msg << "\n";
   results[player.getID()] = msg.str();
   while (results[getEnemyIndex(player)].compare("") == 0) {
      // wait for opponent
      this_thread::sleep_for(chrono::microseconds(100));
   }
   this_thread::sleep_for(chrono::microseconds(100));
   answers[player.getID()] = "0";

   int score = scoreboard[player.getID()];  // gets current players score
   int enemyScore = scoreboard[getEnemyIndex(player)]; // gets the current players enemy score

   msg << "\nYour score: " << score << 
   "\nEnemy score: " << enemyScore << "\n\n";

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
   results[player.getID()] = "";
   while (results[getEnemyIndex(player)].compare("") != 0) {
      // wait for opponent
      this_thread::sleep_for(chrono::microseconds(100));
   }
   sendMsg(player, msg.str());
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

int Server::getEnemyIndex(Player &player) {
   return player.getID()%2 == 1 ? player.getID()+1 : player.getID()-1;
}

void Server::welcomeMessage(Player &player) {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n\n";
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
   "6: Exit Game\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::displayRules(Player &player) {
   stringstream msg;
   msg << "\n~~~~~~ Rules ~~~~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

string Server::displayErr(int code) {
   stringstream msg;
   msg << "Error: ";
   switch(code) {
      case -3:
         msg << "Not a valid name...\n";
         break;
      case -2:
         msg << "Name not found...\n";
         break;
      case -1:
         msg << "Error: Name already taken...\n";
         break;
      case 0:
         msg << "Failed to open leaderboard...\n";
         break;
      default:
         msg << "Code not found (" << code << ")\n";
         break;
   }
   return msg.str();
}