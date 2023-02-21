#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.h"

enum RPS { rock, paper, scissors };

class Client {
   public:
      Client();
      Client(int socket);
      ~Client();
      void makeChoice();
      int convertAnswer(string input);

   private:
      int sd;
      string choice;
      char buffer[8000];
};

#endif