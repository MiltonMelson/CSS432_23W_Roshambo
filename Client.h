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
      int sd;              // the clients socket descriptor
      char buffer[8000];   // the message buffer 

      string menuChoice();
      void displayStats();
      bool reglogPlayer();
      void playGame();
      string makeChoice();
      void bestOutOfThree();
      
   private:
      int sd;
      string choice;
      char buffer[8000];

      int convertAnswer(string &input);
      void convertToLower(string &input);
      void printBuffer();
      void sendMsg(string msg);
      void recvMsg();
};

#endif