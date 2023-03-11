// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Data.h"
#include <fstream>
#include <sstream>
#include <queue>


/**
 * @brief Default constructor.
*/
Data::Data() {}


/**
 * @brief Constructor, sets the server's database filename.
*/
Data::Data(string db) {
   database = db;
}

/**
 * @brief Deconstructor.
*/
Data::~Data() {}

/**
 * @brief Searches the database for a specific player's stats.
 * @param ans A reference to the string to pass the final result.
 * @param name The name to search for in the database.
 * @return Returns a code based on function success.
*/
int Data::getStats(string &ans, const string name) {
   // Check if name is valid
   if (!validEntry(name)) {
      return -3; //Not a valid name
   }

   fstream fin(database);
   // Check if the database can be opened
   if (!fin.is_open()) {
      return 0; // Failed to load leaderboard
   }

   string info[5]; // Holds the information of a line
   string reader; // Holds a single line of the database
   // Reads each line of the database
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      // If name is found, return the stats
      if (info[0].compare(name) == 0) {
         fin.close();
         ans = printStats(info);
         return 1; // Success
      }
   }
   fin.close();
   return -2; // Couldn't find name
}


/**
 * @brief Searches the database for a specific player to update their stats.
 * @param name The name to search for in the database.
 * @param round The number of rounds won.
 * @param draw The number of draws.
 * @return Returns a code based on function success.
*/
int Data::setStats(const string name, const int round, const int draw) {
   // Check if name is valid
   if (!validEntry(name)) {
      return -3; // Not a valid name
   }

   fstream fin(database);
   if (!fin.is_open()) {
      return 0; // Failed to load leaderboard
   }

   ofstream fout("setTemp.txt"); // A new file for copy + pasting
   stringstream msg;
   string info[5]; // Holds the information of a line
   string reader; // Holds a single line of the database
   // Reads each line of the database
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      // If name is found, insert the updated stats
      if(info[0].compare(name) == 0) {
         msg << name << "|" << (stoi(info[1]) + 1) << "|" << (stoi(info[2]) + round) << "|"
             << (stoi(info[3]) + draw) << "|" << name.length();
         string rep = msg.str();
         fout << rep << endl;
      }
      // Otherwise, insert the old stats
      else {
         fout << info[0] << "|" << info[1] << "|" << info[2] << "|"
              << info[3] << "|" << info[4] << endl;
      }
   }
   fin.close();
   fout.close();

   remove(database.c_str()); // Remove the old database file
   rename("setTemp.txt", database.c_str()); // Rename the new file
   return 1; // Success
}


/**
 * @brief Searches the database for the top players.
 * @param ans A reference to the string to pass the final result.
 * @return Returns a code based on function success.
*/
int Data::getBoard(string &ans) {
   fstream fin(database);
   // Checks if the database can be opened
   if (!fin.is_open()) {
      return 0; // Failed to load leaderboard
   }

   priority_queue< pair<int, string> > pq; // Priority queue to order
   string info[5]; // Holds the information of a line
   string reader; // Holds a single line of the database
   // Reads each line of the database
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      int matches = stoi(info[1]);
      string stats = info[0] + ": " + info[1] + " Matches | " + info[2] + " Rounds | " + info[3] +
                     " Draws\n";
      pq.push(make_pair(matches, stats)); // Sorts priority queue by highest matches won
   }
   fin.close();
   
   stringstream msg;
   int i = 1;
   // While priority queue isn't empty
   while (!pq.empty()) {
      // Formatting below 10
      if (i < 10) {
         msg << " " << i << " - ";
      }
      else {
         msg << i << " - ";
      }

      msg << pq.top().second; // Top has highest match wins
      pq.pop();
      // Stop if 20 entries are met or priority queue is empty
      if (i == 20 || pq.empty()) {
         break;
      }
      i++;
   }
   ans += msg.str();
   return 1; // Success
}


/**
 * @brief Searches the database and inserts a new player entry.
 * @param name The name to search for in the database.
 * @return Returns a code based on function success.
*/
int Data::regUser(const string name) {
   // Check if name is valid
   if (!validEntry(name)) {
      return -3; // Not a valid name
   }

   fstream fin(database);
   // Check if the database can be opened
   if (!fin.is_open()) {
      return 0; // Failed to load leaderboard
   }

   ofstream fout("regTemp.txt"); // A new file for copy + pasting
   string info[5]; // Holds the information of a line
   string reader; // Holds a single line of the database
   // Reads each line of the database
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      // If name is found, delete new file
      if(info[0].compare(name) == 0) {
         fin.close();
         fout.close();
         remove("regTemp.txt"); // Delete new file
         return -1; // Name already taken
      }
      // Otherwise, insert the old stats
      else {
         fout << info[0] << "|" << info[1] <<"|" << info[2] << "|"
              << info[3] << "|" << info[0].length() << endl;
      }
   }
   fout << name << "|0|0|0|" << name.length() << endl; // Insert new database entry
   fin.close();
   fout.close();

   remove(database.c_str()); // Remove the old database file
   rename("regTemp.txt", database.c_str()); // Rename the new file
   return 1; // Success
}


/**
 * @brief Searches the database and checks if a player entry exists.
 * @param name The name to search for in the database.
 * @return Returns a code based on function success.
*/
int Data::logUser(const string name) {
   // Check if name is valid
   if (!validEntry(name)) {
      return -3; // Not a valid name
   }

   fstream fin(database);
   // Check if the database can be opened
   if (!fin.is_open()) {
      return 0; // Failed to load leaderboard
   }

   string info[5]; // Holds the information of a line
   string reader; // Holds the single line of the database
   // Reads each line of the database
   while (getline(fin, reader)) {
      readLine(reader, info, sizeof(info)/sizeof(info[0]));
      // If name is found, successfully log in
      if(info[0].compare(name) == 0) {
         fin.close();
         return 1; // Success
      }
   }
   fin.close();
   return -2; // Couldn't find name
}


/**
 * @brief Reads a single line from the database and inserts into info.
 * @param reader Holds a single line of the database.
 * @param info Holds the information of the database line.
 * @param size The array size of info.
*/
void Data::readLine(const string reader, string* info, const int size) {
   string line[size]; // Array of lines holding each stat
   int section = 0;
   for (int i = 0; i < reader.length(); i++) {
      // | = next stat
      if (reader[i] == '|') {
         info[section] = line[section];
         section++;
      }
      else {
         line[section] += reader[i];
      }
   }
   info[size - 1] = line[size - 1];
}


/**
 * @brief Formats the info into a single displayable message.
 * @param info Holds the information of the database line.
 * @return Returns a formatted string.
*/
string Data::printStats(const string* info) const {
   string msg = "Player: " + info[0] +
                "\nMatches won: " + info[1] +
                "\nRounds won: " + info[2] +
                "\nDraws achieved: " + info[3];
   return msg;
}


/**
 * @brief Checks if an string is valid for the database.
 * @param input The string to be checked.
 * @return Returns true if input is less than 21 and is alphanumeric.
*/
bool Data::validEntry(const string input) const {
   // Entries must be more than 0 and less than 21
   if (input.length() == 0 || input.length() >= 20) {
      return false;
   }
   // Checks if each character is alphanumeric
   for (int i = 0; i < input.length(); i++) {
      if(!isalnum(input[i])) {
         return false;
      }
   }
   return true;
}

// Unfinished database encryption/decryption function
/*
string Data::encrypt(const string info, const string pin) const {
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