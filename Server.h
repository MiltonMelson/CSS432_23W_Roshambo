#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

static int scoreboard[5];           // scoreboard to keep track of each players score
static string answers[5];           // String array to store both players answers prior to determining each winner 
static int roster[5];               // current players active in game

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);
      void startGame(Player player);
      void welcomeMessage(Player player);
      void waitForPlayers(Player player);
      void waitForAnswers(Player player);
      int getEnemyIndex(Player player);
      void menuMessage(Player player);
      void displayRules(Player player);
      void determineWinner(Player player);
      string displayBoard();

   private:
      int maxPlayers;
      char buffer[8000];

      void sendMsg(Player player, string msg);
      void recvMsg(Player player);
};

#endif