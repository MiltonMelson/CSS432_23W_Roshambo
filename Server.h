// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

const int numOfPlayers = 101;            // max number of threads/players (adjusted +1 for indexing purposes)         
extern int scoreboard[numOfPlayers];   // the temp scoreboard of each match           
extern string answers[numOfPlayers];   // stores the players answers from each thread 
extern int roster[numOfPlayers];
extern bool threadLock;                 // used to lock all but two threads while determining winners

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);
      void startGame(Player &player);
      void welcomeMessage(Player &player);
      void menuMessage(Player &player);
      void displayRules(Player &player);
      void waitForAnswers(Player &player);
      void determineWinner(Player &player);
      void assignPlayerID(Player &player);
      int getEnemyIndex(Player &player);
      string displayBoard();

   private:
      char buffer[8000];

      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif