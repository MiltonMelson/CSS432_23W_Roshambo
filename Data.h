#ifndef DATA_H
#define DATA_H

#include "Player.h"
#include "Socket.h"

   static string database;

class Data {
   public:
      Data();
      Data(string db);
      ~Data();
      string* getStats(string name);
      void setStats(Player player);
      void getBoard();
      bool regUser();
      bool logUser(Player player);

   private:
      char buffer[8000];

      string* readLine(string reader);
      void printStats(string* info);
      void convertToLower(string &input);
      string encrypt(string info, string pin);
      void sendMsg(Player player, string msg);

};

#endif