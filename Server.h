#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

const int numOfPlayers = 7;           // max number of threads/players (adjusted +1 for indexing purposes)         
extern int scoreboard[numOfPlayers];   // the temp scoreboard of each match           
extern string answers[numOfPlayers];   // stores the clients answers from each thread 
extern int matchReady[numOfPlayers/2];
extern int threadLock;

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);
      void startGame(Player &player);
      void welcomeMessage(Player &player);
      void menuMessage(Player &player);
      void displayRules(Player &player);
      void waitForPlayers(Player &player);
      void waitForAnswers(Player &player);
      void determineWinner(Player &player);
      void prepareMatch(Player &player);
      void leaveMatch(Player &player);
      void makePlayerReady(Player &player, int group);
      int getEnemyIndex(Player &player);
      string displayBoard();

   private:
      char buffer[8000];

      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif