// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef DATA_H
#define DATA_H

#include "Socket.h"

static string database; // Database file name (.txt)

class Data {
   public:
      Data();
      Data(string db);
      ~Data();
      int getStats(string &ans, const string name);
      int setStats(string name, const int round, const int draw);
      int getBoard(string &ans);
      int regUser(string name);
      int logUser(string name);

   private:
      void readLine(const string reader, string* info, const int size);
      string printStats(const string* info) const;
      bool validEntry(const string input) const;
      //string encrypt(const string info, const string pin) const;
};

#endif