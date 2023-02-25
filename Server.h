#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

static int scoreboard[2];           // scoreboard to keep track of each players score
static string answers[2];           // String array to store both players answers prior to determining each winner 
static bool playersReady;

class Server {
   public:
      Server();
      ~Server();
      void startMenu(Player player);
      void startGame(void* info);
      string welcomeMessage();
      string displayRules();
      string displayBoard();
      void waitForPlayers(Player player);
      void waitForAnswers(Player player);
      string determineWinner(Player player);

   private:
      int maxPlayers;
      char buffer[8000];

      void sendMsg(Player player, string msg);
      void recvMsg(Player player);
};

#endif