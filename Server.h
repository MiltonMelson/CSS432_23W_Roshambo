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
extern string users[numOfPlayers];
extern int rounds[numOfPlayers];
extern int matches[numOfPlayers];
extern int draws[numOfPlayers];
extern int roster[numOfPlayers];       // roster of currently active players in match
extern bool threadLock;                // used to lock sendMsg function to prevent multiple threads sending at the same time
extern int messageTimer;               // Used to increase and decrease wait time when threads send messages

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);

   private:
      char buffer[8000];      // message buffer for the server

      void startGame(Player &player, Data &data);
      void welcomeMessage(Player &player);
      void menuMessage(Player &player);
      void displayRules(Player &player);
      int displayStat(Player &player, Data &data);
      int displayBoard(Player &player, Data &data);
      int regPlayer(Player &player, Data &data);
      int logPlayer(Player &player, Data &data);
      void assignPlayerID(Player &player);
      void waitForAnswers(Player &player);
      void determineWinner(Player &player);
      int getEnemyIndex(Player &player);
      string drawChoice(string choice);
      string displayErr(int code);
      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif