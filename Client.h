#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"

enum RPS { rock, paper, scissors };

class Client {
   public:
      Client();
      Client(int socket);
      ~Client();
      string menuChoice();
      void displayStats();
      bool reglogPlayer();
      void playGame();
      void makeChoice();
      void bestOutOfThree();
      
   private:
      int sd;
      string choice;
      char buffer[8000];
      
      int convertAnswer(string &input);
      void convertToLower(string &input);
      void printBuffer();
      void sendMsg(string msg);
      void recvMsg(int sd);
};

#endif