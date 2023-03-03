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
      int menuChoice();
      void playGame();
      string makeChoice();
      void bestOutOfThree();
      
   private:
      int sd;              // the clients socket descriptor
      char buffer[8000];   // the message buffer 

      int convertAnswer(string &input);   // verifies input and converts from string to int
      void convertToLower(string &input); // converts a string to lowercase
      void sendMsg(string msg);           // sends a message through the socket descriptor
      void recvMsg();                     // recieves a message from the socket descriptor
};

#endif