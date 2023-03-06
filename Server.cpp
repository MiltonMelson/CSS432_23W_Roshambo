// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Server.h"
#include <chrono>
#include <sstream>
#include <thread>

int scoreboard[numOfPlayers];       // temp scoreboard of each match        
string answers[numOfPlayers];       // storage for each players provided answers
int roster[numOfPlayers];           // roster of currently active players in match
bool threadLock;                    // used to lock sendMsg function to prevent multiple threads sending at the same time 

/**
 * @brief constructor to initialize all variables 
*/
Server::Server() {
   threadLock = false;
   for (int i = 0; i < numOfPlayers; i++) {
      scoreboard[i] = 0;
      answers[i] = "0";
      roster[i] = 0;
   }
}


// destructor
Server::~Server() {
   cout << "Goodbye..." << endl;
}


/**
 * @brief This is the entry point for each player.
 * The server will send a welcome and menu messages, and then wait the players menu selection.
 * @param info the thread info for each player provided from the thread function in socket.cpp
*/
void Server::startMenu(void* info) {
   Data data("textfile.txt");
   Player player = *(Player*)info;  // store info into player object
   bool exit = false;               // if player want to exit the game
   int ans = 0;                     // players selection

   welcomeMessage(player);          // sends the welcome message to the player

   while (!exit) {
      menuMessage(player);
      recvMsg(player);
      ans = atoi(buffer);
      int code;
      switch(ans) {
         case 1:
            code = 1;
            displayRules(player);
            break;
         case 2:
            code = displayStat(player, data);
            break;
         case 3:
            code = displayBoard(player, data);
            break;
         case 4:
            code = regPlayer(player, data);
            if (code == 1) {
               startGame(player);
               this_thread::sleep_for(chrono::microseconds(15000000));
            };
            break;
         case 5:
            code = logPlayer(player, data);
            if (code == 1) {
               startGame(player);
               this_thread::sleep_for(chrono::microseconds(15000000));
            };
            break;
         case 6:
            code = 1;
            startGame(player);
            player.setGuest();
            this_thread::sleep_for(chrono::microseconds(15000000));
            break;
         case 7:
            code = 1;
            roster[player.getID()] = 0;
            exit = true;
            break;
         default:
            continue;
      }
      if (code != 1) {
         sendMsg(player, displayErr(code));
      }  
   }   
   close(player.getSD());  // close players socket descriptor
}


/**
 * @brief Starts the game for the provided player object
 * @param player A reference to the player created in startMenu function
*/
void Server::startGame(Player &player) {
   assignPlayerID(player);    // assigns the current player a temp number for the match

   cout << "Player " << player.getID() << " has joined the game!" << endl;

   int score = 0;             // the current players score
   int enemyScore = 0;        // the enemies score
   int round = 1;             // the current round

   // best 2 out of 3
   while (score < 2 && enemyScore < 2) {
      stringstream msg;
      msg << "\nRound " << round++ << endl;  // message for each round
      sendMsg(player, msg.str());            // send the message to the player
      waitForAnswers(player);                // waits answers from player and enemy 
      determineWinner(player);               // determines the winner for current round
      score = scoreboard[player.getID()];    // gets score
      enemyScore = scoreboard[getEnemyIndex(player)]; // gets the enemy score
   }

   // removes the player from the roster and scoreboard
   roster[player.getID()] = 0;
   cout << "Player " << player.getID() << " has left the game!" << endl;
   while (roster[getEnemyIndex(player)] != 0) {
      // wait for opponent to exit before resetting scoreboard 
   }
   scoreboard[player.getID()] = 0;
   if (player.getID()%2==0) {
      this_thread::sleep_for(chrono::microseconds(1000000));
   }
   player.setID(0);     // set players id to 0 for inactive
}


/**
 * @brief Creates and sends the welcome message to the player
 * @param player A reference to the player passed from startMenu function
*/
void Server::welcomeMessage(Player &player) {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n" << endl;
   string temp = msg.str();
   sendMsg(player, temp);
}


/**
 * @brief Creates and sends the menu message to the player
 * @param player A reference to the player passed from startMenu function
*/
void Server::menuMessage(Player &player) {
   stringstream msg;
   msg << "Main Menu:\n" <<
   "1: View the rules\n" <<
   "2: View the stats of an existing player\n" <<
   "3: View the leaderboard\n" << 
   "4: Register as a new player\n" <<
   "5: Log in as an existing player\n" << 
   "6: Play as a guest\n" <<
   "7: Exit Game\n" << endl;
   string temp(msg.str());
   sendMsg(player, temp);
}


/**
 * @brief Creats and sends the message for the rules of the game to the player
 * @param player A reference to the player passed from startMenu function
*/
void Server::displayRules(Player &player) {
   stringstream msg;
   msg << "\n~~~ Rules ~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n" << endl;
   string temp = msg.str();
   sendMsg(player, temp);
}


/**
 * @brief Assigns the player an ID for indexing in global variables.
 * The player will be assigned the index based on available slots in roster.
 * If a player is assigned to a slot and it does not get an enemy in 15 secs it
 * will reset their ID and remove them from the roster and search for a new ID in roster.
 * If the enemy shows up before 15 secs it will break and both players will start the match
 * @param player A reference to the player passed from the startGame function
*/
void Server::assignPlayerID(Player &player) {
   while (true) {

      // search roster for open slot
      for (int i = 1; i < numOfPlayers; i++) {
         if (roster[i] == 0) {
            roster[i] = 1;
            player.setID(i);
            break;
         }
      }

      // start timer for thread to possibly reset and search again
      int timer = 0;
      while (roster[getEnemyIndex(player)] == 0) {
         // wait for opponent
         this_thread::sleep_for(chrono::microseconds(1000000));
         ++timer;
         if (timer >= 15) {
            roster[player.getID()] = 0;
            player.setID(0);
            break;
         }
      }

      // if player ID is 0 then no opponent found restart search
      if (player.getID() == 0) {
         continue;
      }
      // else opponent found leave function
      else {
         return;
      }
   }
}



/**
 * @brief Assigns the player an ID for indexing in global variables.
 * The player will be assigned the index based on available slots in roster.
 * If a player is assigned to a slot and it does not get an enemy in 15 secs it
 * will reset their ID and remove them from the roster and search for a new ID in roster.
 * If the enemy shows up before 15 secs it will break and both players will start the match
 * @param player A reference to the player passed from the startGame function
*/
void Server::assignPlayerID(Player &player) {
   while (true) {

      // search roster for open slot
      for (int i = 1; i < numOfPlayers; i++) {
         if (roster[i] == 0) {
            roster[i] = 1;
            player.setID(i);
            break;
         }
      }

      // start timer for thread to possibly reset and search again
      int timer = 0;
      while (roster[getEnemyIndex(player)] == 0) {
         // wait for opponent
         this_thread::sleep_for(chrono::microseconds(1000000));
         ++timer;
         if (timer >= 15) {
            roster[player.getID()] = 0;
            player.setID(0);
            break;
         }
      }

      // if player ID is 0 then no opponent found restart search
      if (player.getID() == 0) {
         continue;
      }
      // else opponent found leave function
      else {
         return;
      }
   }
}


int Server::displayStat(Player &player, Data &data) {
   string ans;

   sendMsg(player, "Type the name for the player's stats you want to view: ");
   recvMsg(player);

   int resp = data.getStats(ans, buffer);
   if (resp == 1) {
      sendMsg(player, ans);
   }
   return resp;
}

int Server::displayBoard(Player &player, Data &data) {
   string ans = "~~~ Roshambo Leaderboard! ~~~\n";
   int resp = data.getBoard(ans);
   if (resp == 1) {
      sendMsg(player, ans);
   }
   return resp;
}

int Server::regPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Type in your username (alphanumeric only, max 20 chars.): ");
   recvMsg(player);

   int resp = data.regUser(name);
   if (resp == 1) {
      sendMsg(player, "Welcome to Roshambo, " + name + "!");
      player.setName(name.c_str());
   }

   return resp;
}

int Server::logPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Username: ");
   recvMsg(player);

   int resp = data.logUser(name);
   if (resp == 1) {
      sendMsg(player, "Welcome back, " + name + "!");
   }

   return resp;
}


/**
 * @brief Recieves the answers from each player and makes them wait for their
 * opponent to also answer before proceeding to determine the winners.
 * @param player A reference to player passed from the startGame function
*/
void Server::waitForAnswers(Player &player) {
   recvMsg(player);           // wait for players to input answer
   player.setChoice(buffer);  // sets players choice
   cout << "Player " << player.getID() << " Choice was: " << player.getChoice() << endl;
   answers[player.getID()] = player.getChoice();
   while (answers[getEnemyIndex(player)].compare("0") == 0) {
      // wait for opponent to answer
   }
}


/**
 * @brief Determines the winner between current players in match based on rps logic.
 * Then sends the corresponding win/lose message to each player. If there is a winner
 * both players will recieve the terminating message and client.cpp will end bestOutOfThree loop.
 * @param player A reference to player passed from the startGame function
*/
void Server::determineWinner(Player &player) {
   string p1 = answers[player.getID()];         // the current players answer
   string p2 = answers[getEnemyIndex(player)];  // the current players enemy answer
   
   stringstream msg;                            // the message to be sent after determining the winner

   // if player disconnects mid game
   if (p2.compare("") == 0) {
      cout << "Player " << getEnemyIndex(player) << " has disconnected!" << endl;
      msg << "\nOpponent Disconnected...\n\n You Win the Match!\n\ndisconnect";
      sendMsg(player, msg.str());            // send terminating message to player
      scoreboard[player.getID()] = 2;        // increase scoreboard to break loop in startGame
      answers[player.getID()] = "0";         // reset answers for player
      answers[getEnemyIndex(player)] = "0";  // reset enemies answers
      scoreboard[getEnemyIndex(player)] = 0; // reset disconected players scoreboard
      roster[getEnemyIndex(player)] = 0;     // reset enemies roster
      return;
   }

   msg << "\n\n" << drawChoice(p1) << "\n\n    vs    \n\n" << drawChoice(p2) << "\n\n";

   if (p1.compare("rock") == 0) {               // player 1 picks rock
      if (p2.compare("paper") == 0) {           // player 2 picks paper
         msg << "You Lost\n\nPaper covers Rock!";
      }
      else if (p2.compare("scissors") == 0) {   // player 2 picks scissors
         msg << "You Won\n\nRock smashes Scissors!";
         scoreboard[player.getID()]++;          // player 1 won so increase scoreboard
      }
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }
   // player 1 picks paper
   else if (p1.compare("paper") == 0) {
      // player 2 picks Scissors
      if (p2.compare("scissors") == 0) {
         msg << "You Lost\n\nScissors cuts Paper!";
      }
      else if (p2.compare("rock") == 0) {       // player 2 picks Rock
         msg << "You Won\n\nPaper covers Rock!";
         scoreboard[player.getID()]++;           // player 1 won so increase scoreboard
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }
   // player 1 picks Scissors
   else {
      // player 2 picks Rock
      if (p2.compare("rock") == 0) {
         msg << "You Lost\n\nRock smashes Scissors!";
      }
      else if (p2.compare("paper") == 0) {      // player 2 picks Paper
         msg << "You Won\n\nScissors cuts Paper!";
         scoreboard[player.getID()]++;           // player 1 won so increase scoreboard
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         if (!player.isGuest()) {
            player.setDraw();
         }
      }
   }

   // sleep thread 1 or 2 doesnt matter
   // this will allow them both to catch up prior to determining the score
   this_thread::sleep_for(chrono::microseconds(100));

   int score = scoreboard[player.getID()];  // gets current players score
   int enemyScore = scoreboard[getEnemyIndex(player)]; // gets the current players enemy score

   msg << "\n\nYour score: " << score << 
   "\nEnemy score: " << enemyScore << "\n\n";

   // if either has a score of 2 then they won
   if (score == 2 || enemyScore == 2) {
      if (score == 2) {             // if its the current player
         msg << "You Won the Match!\n\n";
         if (!player.isGuest()) {
            player.setMatch();
         }
      }
      else {                        // if its the enemy
         msg << "You Lost the Match!\n\n";
      }
      if (!player.isGuest()) {
         player.setRound(scoreboard[0]);
      }
      msg << "Exit";                // terminating message for client.cpp
   }
   sendMsg(player, msg.str());      // sends the enitre message
   answers[player.getID()] = "0";   // reset answers for each player
}


/**
 * @brief Gets the current players enemy index.
 * @param player A reference to the current player
 * @return returns the index of the current players enemy index
*/
int Server::getEnemyIndex(Player& player) {
   // if the current player ID is 1 then its enemy is 2
   // if the current player ID is 2 then its enemy is 1
   // this applies to 3 and 4, and so on
   return player.getID()%2 == 0 ? player.getID()-1 : player.getID()+1;
}


/**
 * @brief Draws an ASCII hand signal for rock, paper, or scissors.
 * @param choice rock, paper, scissors
 * @return the text based animations of rock, paper, scissors
*/
string Server::drawChoice(string choice) {
   if (choice.compare("rock") == 0) {
      return "    ______\n"
             "---'   ___)_\n"
             "      (_____)\n"
             "      (_____)\n"
             "      (____)\n"
             "---.__(___)\n";
   }
   else if (choice.compare("paper") == 0) {
      return "    _______\n"
             "---'   _____)___\n"
             "          ______)\n"
             "          _______)\n"
             "         _______)\n"
             "---.__________)\n";

   }
   else { // scissors
      return "    _______\n"
             "---'   ____)____\n"
             "          ______)\n"
             "       __________)\n"
             "      (____)\n"
             "---.__(___)\n";
   }

}


/**
 * @brief Sends a message to the player object
 * @param player the player the message is getting sent to
 * @param msg the message that is being sent
*/
void Server::sendMsg(Player &player, string msg) {
   // sleeps a thread based on their current player ID in hopes of offsetting their entry time
   // this should allow enough time for 1 thread to get the threadLock before the other skips the while loop
   this_thread::sleep_for(chrono::microseconds(player.getID()*100));
   
   // This will lock all threads while one thread is sending a message
   while (threadLock) {
      // each thread will wait for different times so they dont all get released at once and hog the threadLock
      this_thread::sleep_for(chrono::microseconds(player.getID()*100));
   }

   // sets the threadLock so other threads will wait
   threadLock = true;

   // sends the actual message
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);

   // resets the threadLock, notifying other threads
   threadLock = false;
}


/**
 * @brief Recieves a message from the players socket descriptor
 * @param player The player the message will be recieved from
*/
void Server::recvMsg(Player &player) {
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
}

int Server::getEnemyIndex(Player &player) {
   return player.getID()%2 == 1 ? player.getID()+1 : player.getID()-1;
}

void Server::welcomeMessage(Player &player) {
   stringstream msg;
   msg << "\n---------------- Welcome to Roshambo ----------------\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::menuMessage(Player &player) {
   stringstream msg;
   msg << "Main Menu:\n" << 
   "1: View the rules\n" << 
   "2: View the leaderboard\n" << 
   "3: Register as a new player\n" <<
   "4: Log in as an existing player\n" << 
   "5: Play as a guest\n" <<
   "6: Exit Game\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

void Server::displayRules(Player &player) {
   stringstream msg;
   msg << "\n~~~~~~ Rules ~~~~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n\n";
   string temp = msg.str();
   sendMsg(player, temp);
}

string Server::displayErr(int code) {
   stringstream msg;
   msg << "Error: ";
   switch(code) {
      case -3:
         msg << "Not a valid name...\n";
         break;
      case -2:
         msg << "Name not found...\n";
         break;
      case -1:
         msg << "Error: Name already taken...\n";
         break;
      case 0:
         msg << "Failed to open leaderboard...\n";
         break;
      default:
         msg << "Code not found (" << code << ")\n";
         break;
   }
   return msg.str();
}