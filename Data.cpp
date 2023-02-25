#include "Data.h"
#include <fstream>
#include <queue>

Data::Data() {}

Data::Data(string db) {
   database = db;
}

Data::~Data() {}

string* Data::getStats(string name) {
   string* info;
   fstream fin(database);
   if (!fin.is_open()) {
      info[0] = "-1";
      info[1] = "Error: Could not load leaderboard...";
      return info;
   }

   string reader;
   int section = 0;
   while (getline(fin, reader)) {
      for (int i = 0; i < reader.length(); i++) {
         info = readLine(reader);
      }
      if (info[0].compare(name)) {
         fin.close();
         return info;
      }
      else {
         section = 0;
      }
   }
   fin.close();
   return info;
}

void Data::setStats(Player player) {
   string* info;
   string rep = player.getName() + '|' + player.getRound() + '|'+ player.getMatch() + '|' +
                player.getDraw();
   fstream fin(database);
   if (!fin.is_open()) {
      info[0] = "-1";
      info[1] = "Error: Could not load leaderboard...";
      return;
   }

   ofstream fout("temp.txt");

   string reader;
   int section = 0;
   while (getline(fin, reader)) {
      for (int i = 0; i < reader.length(); i++) {
         info = readLine(reader);
      }
      if(info[0].compare(player.getName())) {
         fout << rep << endl;
      }
      else {
         fout << info[0] << "|" << info[1] << info[2] << "|" << info[3] << "|" << endl;
      }
   }

   fin.close();
   fout.close();
   remove(database.c_str());
   rename("temp.txt", database.c_str());
}

void Data::getBoard() {
   string* info;
   fstream fin(database);
   if (!fin.is_open()) {
      info[0] = "-1";
      info[1] = "Error: Could not load leaderboard...";
      return;
   }
   priority_queue< pair<int, string*> > pq;
   
   string reader;
   int section = 0;
   while (getline(fin, reader)) {
      info = readLine(reader);
      pq.push(make_pair(stoi(info[3]), info));
   }
   fin.close();
}

bool Data::regUser() {
   string* info;
   fstream fin(database);
   if (!fin.is_open()) {
      info[0] = "-1";
      info[1] = "Error: Could not load leaderboard...";
      return false;
   }
   ofstream fout("temp.txt");
   string name;

   string reader;
   int section = 0;
   while (getline(fin, reader)) {
      for (int i = 0; i < reader.length(); i++) {
         info = readLine(reader);
      }
      if(info[0].compare(name)) {
         fin.close();
         fout.close();
         remove("temp.txt");
         return false;
      }
      else {
         fout << info[0] << "|" << info[1] << info[2] << "|" << info[3] << "|" << endl;
      }
   }
   fout << name << "|0|0|0" << endl;

   fin.close();
   fout.close();
   remove(database.c_str());
   rename("temp.txt", database.c_str());
   return true;
}

string* Data::readLine(string reader) {
   string info[4];
   int section = 0;
   for (int i = 0; i < reader.length(); i++) {
      if (reader[i] == '|') {
         section++;
      }
      else {
         info[section] += reader[i];
      }
   }
   return info;
}

bool logUser(Player player) {
   return false;
}

void Data::printStats(string* info) {
   string msg = "Player: " + info[0] + "\nRounds won: " + info[1] + "\nMatches won: " + info[2] +
                "\nDraws achieved: " + info[3];
}

void Data::convertToLower(string &input) {
   for (int i = 0; i < input.length(); i++) {
      if (input[i] > 'A' && input[i] < 'Z') {
         input[i] = input[i] + 32;
      }
   }
}

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

void Data::sendMsg(Player player, string msg) {
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);
}