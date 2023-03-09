// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Data.h"
#include <fstream>
#include <sstream>
#include <queue>

// Need to fully test

Data::Data() {}

Data::Data(string db) {
   database = db;
}

Data::~Data() {}

// Error codes
// -3: Not a valid name
// -2: Couldn't find name
// -1: Name already taken
//  0: Failed to load leaderboard
//  1: Success

int Data::getStats(string &ans, string name) {
   if (!validEntry(name)) {
      //sendMsg(player, "Error: Not a valid name...\n");
      return -3;
   }

   string info[5];
   fstream fin(database);
   if (!fin.is_open()) {
      //sendMsg(player, "Error: Failed to load leaderboard...\n");
      return 0; // Failed to open
   }


   string reader;
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      if (info[0].compare(name) == 0) {
         fin.close();
         ans = printStats(info); // Found name
         return 1;
      }
   }
   fin.close();
   //sendMsg(player, "Error: Name not found...\n");
   return -2; // Couldn't find name
}

int Data::setStats(string name, int match, int round, int draw) {
   cout << "Setting " << name << endl;
   if (!validEntry(name)) {
      //sendMsg(player, "Error: Not a valid name...\n");
      return -2;
   }
   stringstream msg;
   string info[5];
   fstream fin(database);
   if (!fin.is_open()) {
      //sendMsg(player, "Error: Failed to load leaderboard...\n");
      return 0; // Failed to open
   }

   ofstream fout("temp.txt");

   string reader;
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      if(info[0].compare(name) == 0) {
         match += stoi(info[1]);
         round += stoi(info[2]);
         draw += stoi(info[3]);

         msg << name << "|" << match << "|" << round << "|"
             << draw << "|" << name.length();
         string rep = msg.str();
         cout << rep << endl;
         fout << rep << endl;
      }
      else {
         fout << info[0] << "|" << info[1] << "|" << info[2] << "|"
              << info[3] << "|" << info[4] << endl;
      }
   }

   fin.close();
   fout.close();
   remove(database.c_str());
   rename("temp.txt", database.c_str());
   //sendMsg(player, "Player data uploaded to leaderboard!\n");
   return 1;
}

int Data::getBoard(string &ans) {
   string info[5];
   fstream fin(database);
   if (!fin.is_open()) {
      //sendMsg(player, "Error: Failed to load leaderboard...\n");
      return 0; // Failed to open
   }
   priority_queue< pair<int, string> > pq;
   int matches;
   string stats;
   string reader;
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      matches = stoi(info[1]);
      stats = info[0] + ": " + info[1] + " Matches | " + info[2] + " Rounds | " + info[3] +
              " Draws\n";
      pq.push(make_pair(matches, stats));
   }
   fin.close();
   
   stringstream msg;
   int i = 1;
   while (!pq.empty()) {
      if (i < 10) {
         msg << " " << i << " - ";
      }
      else {
         msg << i << " - ";
      }
      msg << pq.top().second;
      pq.pop();
      if (i == 20 || pq.empty()) {
         break;
      }
      i++;
   }
   ans += msg.str();
   return 1;
}

int Data::regUser(string name) {
   if (!validEntry(name)) {
      //sendMsg(player, "Error: Not a valid name...\n");
      return -3;
   }

   string info[5];
   fstream fin(database);
   if (!fin.is_open()) {
      //sendMsg(player, "Error: Failed to load leaderboard...\n");
      return 0; // Failed to open
   }
   ofstream fout("temp.txt");

   string reader;
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      if(info[0].compare(name) == 0) {
         fin.close();
         fout.close();
         remove("temp.txt");
         //sendMsg(player, "Error: Name already taken...\n");
         return -1; // Name already taken
      }
      else {
         fout << info[0] << "|" << info[1] <<"|" << info[2] << "|"
              << info[3] << "|" << info[0].length() << endl;
      }
   }
   fout << name << "|0|0|0|" << name.length() << endl;

   fin.close();
   fout.close();
   remove(database.c_str());
   rename("temp.txt", database.c_str());
   //sendMsg(player, "New player registered - Welcome " + info[0] + "!\n");
   return 1; // Inserted new entry
}

int Data::logUser(string name) {
   if (!validEntry(name)) {
      //sendMsg(player, "Error: Not a valid name...\n");
      return -3;
   }

   string info[5];
   fstream fin(database);
   if (!fin.is_open()) {
      //sendMsg(player, "Error: Failed to load leaderboard...\n");
      return 0; // Failed to open
   }

   string reader;
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      if(info[0].compare(name) == 0) {
         fin.close();
         //sendMsg(player, "Welcome back " + info[0] + "!\n");
         return 1; // Found name, log in
      }
   }
   fin.close();
   //sendMsg(player, "Error: Name not found...\n");
   return -2; // Couldn't find name
}

void Data::readLine(string reader, string* info, int length) {
   string line[length];
   int section = 0;
   for (int i = 0; i < reader.length(); i++) {
      if (reader[i] == '|') {
         info[section] = line[section];
         section++;
      }
      else {
         line[section] += reader[i];
      }
   }
   info[length - 1] = line[length - 1];
}

string Data::printStats(string* info) {
   string msg = "Player: " + info[0] +
                "\nMatches won: " + info[1] +
                "\nRounds won: " + info[2] +
                "\nDraws achieved: " + info[3];
   return msg;
}

void Data::convertToLower(string &input) {
   for (int i = 0; i < input.length(); i++) {
      if (input[i] > 'A' && input[i] < 'Z') {
         input[i] = input[i] + 32;
      }
   }
}

bool Data::validEntry(string input) {
   if (input.length() == 0 || input.length() > 20) {
      return false;
   }
   for (int i = 0; i < input.length(); i++) {
      if(!isalnum(input[i])) {
         return false;
      }
   }
   return true;
}

/*
string Data::encrypt(string info, string pin) {
   string output;
   int sum = 0;
   char key;
   for (int i = 0; i < pin.length(); i++) {
      sum += pin[i];
   }
   key = char(sum % 126);

   for (int i; i < info.length(); i++) {
      output[i] = info[i] ^ key;
   }
   return output;
}
*/