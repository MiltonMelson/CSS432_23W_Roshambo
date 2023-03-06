#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

const int numOfPlayers = 13;           // max number of threads/players (adjusted +1 for indexing purposes)         
extern int roster[numOfPlayers];       // the current roster of who is online
extern int scoreboard[numOfPlayers];   // the temp scoreboard of each match           
extern string answers[numOfPlayers];   // stores the clients answers from each thread 
extern bool threadLock;

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);
      void startGame(Player &player);
      void welcomeMessage(Player &player);
      void menuMessage(Player &player);
      void displayRules(Player &player);
      int displayStat(Player &player, Data &data);
      int displayBoard(Player &player, Data &data);
      int regPlayer(Player &player, Data &data);
      int logPlayer(Player &player, Data &data);
      void waitForPlayers(Player &player);
      void waitForAnswers(Player &player);
      void determineWinner(Player &player);
      int getEnemyIndex(Player &player);

   private:
      char buffer[8000];

      string displayErr(int code);
      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif