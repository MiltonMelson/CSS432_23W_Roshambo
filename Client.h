// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"

enum RPS { rock, paper, scissors };

class Client {
   public:
      Client();
      Client(int socket);
      ~Client();
      
   private:
      int sd;              // The clients socket descriptor
      string choice;       // The clients choice
      char buffer[1000];   // The message buffer for the client

      string menuChoice();
      void displayStats();
      bool reglogPlayer();
      void playGame();
      string makeChoice();
      void bestOutOfThree();
      int convertAnswer(string &input);
      void convertToLower(string &input);
      void printBuffer();
      void sendMsg(string msg);
      void recvMsg();
};

#endif