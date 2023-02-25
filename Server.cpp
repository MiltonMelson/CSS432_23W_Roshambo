#include "Server.h"
#include <sstream>

Server::Server() {
   maxPlayers = 2;         // current player capacity of the game
   answers[0] = "0";       // set answers to 0
   answers[1] = "0";    
   scoreboard[0] = 0;      // sets temp scoreboard to 0
   scoreboard[1] = 0;
   playersReady = false;   // flag to tell both players have arrived
}

Server::~Server() {
   cout << "\n\nGoodbye..." << endl;
}

// In progress
void Server::startMenu(Player player) {
   bool wait = true;
   int ans = 0;

   while (wait) {
      sendMsg(player, welcomeMessage());
      recvMsg(player);
      ans = atoi(buffer);
      switch(ans) {
         case 1:
            sendMsg(player, displayRules());
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
            player.setGuest();
            wait = false;
            break;
         default:
            continue;
      }
      
   }
}

void Server::startGame(void* info) {
   Player player = *(Player*)info;  // store info into player
   startMenu(player);
   waitForPlayers(player); // waits for both players to join the game before 

   // gets the current players score and their enemies score
   int score = scoreboard[player.getID()-1];
   int enemyScore = scoreboard[(player.getID())%2];

   // best 2 out of 3
   while (score < 2 && enemyScore < 2) {

      waitForAnswers(player); // waits for both players to submit answers before proceeding 

      stringstream msg;
      msg << determineWinner(player);  // determine the winner and the resulting message for each player

      score = scoreboard[player.getID()-1];  // gets current players score
      enemyScore = scoreboard[(player.getID())%2]; // gets the current players enemy score

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
   close(player.getSD());  // close players socket descriptor
}

string Server::welcomeMessage() {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n\n" <<
   "Main Menu:\n> 1: View the rules\n2: View an existing player's stats \n" <<
   "> 3: View the leaderboard\n> 4: Register as a new player\n" <<
   "> 5: Log in as an existing player\n> 6: Play as a guest\n\n";
   string temp = msg.str();
   return temp;
}

string Server::displayRules() {
   stringstream msg;
   msg << "\nEach player will pick either rock, paper, or scissors.\n\n" <<
   "How To Win: \n\n(Rock beats Scissors, Scissors beats Paper, Paper beats Rock)\n\n\n";
   string temp = msg.str();
   return temp;
}

// In progress
string Server::displayBoard() {
   stringstream msg;
   string temp;
   return temp;
}

void Server::waitForPlayers(Player player) {
   cout << "Player " << player.getID() << " has joined the game!" << endl;
   // if the second player has arrived
   if (player.getID() == 2) {
      playersReady = true;
   }
   while (playersReady == false) {
      // wait for player 2 
   }

}

void Server::waitForAnswers(Player player) {
   recvMsg(player); // wait for players to input answer
   player.setChoice(buffer);
   cout << "Player " << player.getID() << " Choice was: " << player.getChoice() << endl;
   while (answers[0] == "0" || answers[1] == "0") {
      // wait for players to pick 
      answers[player.getID()-1] = player.getChoice();
   }
   // if one of the players leave send last message with Exit flag at end of message
   if (answers[0] == "" && answers[1] != "") {
      sendMsg(player, "Player 1 has left the game! You Win!Exit");
   }
   if (answers[0] != "" && answers[1] == "") {
      sendMsg(player, "Player 2 has left the game! You Win!Exit");
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
         player.setDraw();
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
         player.setDraw();
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
         player.setDraw();
      }
   }
   answers[0] = "0";
   answers[1] = "0";
   return msg;
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