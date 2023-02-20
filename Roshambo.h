#ifndef ROSHAMBO_H
#define ROSHAMBO_H

#include "Socket.h"

// Structure to hold thread data allowing access thread info during each match
struct playerInfo {      
   int sd;
   int ID;
   char* choice;
};

enum RPS { rock, paper, scissors };
static int* scoreboard[2];         // scoreboard to keep track of each players score
static string answers[2];          // String array to store both players answers prior to determining each winner 

class Roshambo {
   public:
      Roshambo();
      ~Roshambo();
      void startGame(void* info);
      void enterLobby();
      void determineWinner();
      void welcomeMessage();
      void displayRules();
      int convertAnswer(string input);
      int userChoice();
      char* showChoice(const RPS choice);

   private:
      playerInfo* player;
      int maxPlayers;
};

#endif