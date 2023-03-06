// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef DATA_H
#define DATA_H

#include "Socket.h"

   static string database;

class Data {
   public:
      Data();
      Data(string db);
      ~Data();
      int getStats(string &ans, string name);
      int setStats(string name, int match, int round, int draw);
      int getBoard(string &ans);
      int regUser(string name);
      int logUser(string name);

   private:
      string* readLine(string reader);
      string printStats(string* info);
      void convertToLower(string &input);
      bool validEntry(string input);
      //string encrypt(string info, string pin);

};

#endif