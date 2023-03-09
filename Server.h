// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef SERVER_H
#define SERVER_H

#include "Player.h"
#include "Data.h"
#include "Socket.h"

const int numOfPlayers = 101;          // Current max players can go higher unlikely to need to         
extern bool roster[numOfPlayers];      // Roster of currently active players in match
extern string users[numOfPlayers];     // Storage for each players usernames
extern string answers[numOfPlayers];   // Storage for each players provided answers
extern int scoreboard[numOfPlayers];   // Temp. scoreboard of each match
extern int draws[numOfPlayers];        // Storage for each player's draws
extern bool win[numOfPlayers];         // Storage if a player won a match
extern bool threadLock;                // Used during thread critical sections
extern int messageTimer;               // Used to alter wait time during critical sections

class Server {
   public:
      Server();
      ~Server();
      void startMenu(void* info);

   private:
      char buffer[1000];               // Message buffer for the server

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
      int getEnemyIndex(Player &player) const;
      string drawChoice(const string choice) const;
      string displayErr(const int code) const;
      void sendMsg(Player &player, string msg);
      void recvMsg(Player &player);
};

#endif