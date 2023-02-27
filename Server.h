#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

extern int roster[3];
extern int scoreboard[3];           
extern string answers[3];
extern mutex mtx;
extern bool ready;
extern condition_variable cv;           

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
      int getEnemyIndex(Player &player);
      string displayBoard();

   private:
      int maxPlayers;
      char buffer[8000];

      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif