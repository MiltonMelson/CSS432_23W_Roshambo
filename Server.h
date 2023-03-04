// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

const int numOfPlayers = 101;          // current max players can go higher unlikely to need to         
extern int scoreboard[numOfPlayers];   // temp scoreboard of each match           
extern string answers[numOfPlayers];   // storage for each players provided answers
extern int roster[numOfPlayers];       // roster of currently active players in match
extern bool threadLock;                // used to lock sendMsg function to prevent multiple threads sending at the same time

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);

   private:
      char buffer[8000];      // message buffer for the server

      void startGame(Player &player);
      void welcomeMessage(Player &player);
      void menuMessage(Player &player);
      void displayRules(Player &player);
      string displayBoard();
      void assignPlayerID(Player &player);
      void waitForAnswers(Player &player);
      void determineWinner(Player &player);
      int getEnemyIndex(Player &player);
      string drawChoice(string choice);
      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif