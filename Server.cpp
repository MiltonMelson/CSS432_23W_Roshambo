// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Server.h"
#include <chrono>
#include <sstream>
#include <thread>

bool roster[numOfPlayers];    // Roster of currently active players in match
string users[numOfPlayers];   // Storage for each players usernames
string answers[numOfPlayers]; // Storage for each players provided answers
int scoreboard[numOfPlayers]; // Temp. scoreboard of each match
int draws[numOfPlayers];      // Storage for each player's draws
bool win[numOfPlayers];       // Storage if a player won a match
bool threadLock;              // Used during thread critical sections
int messageTimer;             // Used to alter wait time during critical sections


/**
 * @brief Constructor to initialize all variables.
*/
Server::Server() {
   messageTimer = 0;
   threadLock = false;
   for (int i = 0; i < numOfPlayers; i++) {
      roster[i] = false;
      users[i] = "";
      answers[i] = "0";
      draws[i] = 0;
      win[i] = false;
   }

   cout << "[Game] Waiting for players" << endl;
}


/**
 * @brief Destructor.
*/
Server::~Server() {
   cout << "[Game] Server shutting down... Goodbye!" << endl;
}


/**
 * @brief This is the entry point for each player. The server will send a welcome and menu messages,
 * and then wait the players menu selection.
 * @param info The thread info for each player provided from the thread function in socket.cpp.
*/
void Server::startMenu(void* info) {
   Data data("database.txt");
   Player player = *(Player*)info; // Store info into player object
   player.setID(0);
   bool exit = false; // If player want to exit the game
   int ans = 0; // Player's option

   welcomeMessage(player); // Sends the welcome message to the player
   while (!exit) {
      menuMessage(player); // Sends the menu message to the player
      recvMsg(player); // Receives option
      ans = atoi(buffer);
      int code; // Error code
      switch(ans) {
         case 1: // Display game rules
            code = 1;
            displayRules(player);
            break;
         case 2: // Display stats of a selected player
            code = displayStat(player, data);
            this_thread::sleep_for(chrono::microseconds(1000000));
            break;
         case 3: // Display the leaderboard
            code = displayBoard(player, data);
            this_thread::sleep_for(chrono::microseconds(1000000));
            break;
         case 4: // Register a player
            code = regPlayer(player, data);
            if (code == 1) {
               startGame(player, data);
               this_thread::sleep_for(chrono::microseconds(15000000));
            };
            break;
         case 5: // Log in as an existing player
            code = logPlayer(player, data);
            if (code == 1) {
               startGame(player, data);
               this_thread::sleep_for(chrono::microseconds(15000000));
            };
            break;
         case 6: // Play as guest
            code = 1;
            player.setGuest();
            startGame(player, data);
            this_thread::sleep_for(chrono::microseconds(15000000));
            break;
         case 7: // Exit game
            code = 1;
            roster[player.getID()] = false;
            exit = true;
            break;
         default:
            continue;
      }

      // 1 = success
      if (code != 1) {
         sendMsg(player, displayErr(code));
         this_thread::sleep_for(chrono::microseconds(100000));
      }
   }  
   close(player.getSD());  // Close players socket descriptor
}


/**
 * @brief Starts the game for the provided player object.
 * @param player A reference to the player created in startMenu function.
 * @param data A reference to the database created in startMenu function.
*/
void Server::startGame(Player &player, Data &data) {
   assignPlayerID(player); // Assigns the current player a temp number for the match

   // Initialising players
   // Assign names according to guest or not
   if (!player.isGuest()) {
      users[player.getID()] = player.getName();
   }
   else {
      player.setName(("Guest " + to_string(player.getID())).c_str());
      users[player.getID()] = player.getName();
   }
   draws[player.getID()] = 0;
   win[player.getID()] = false;

   cout << "[Game] " << users[player.getID()] << " (" << player.getID() << ") has joined." << endl;

   int score = 0; // Current player's score
   int enemyScore = 0; // Opponent's score
   int round = 1; // Round count

   // best 2 out of 3
   while (score < 2 && enemyScore < 2) {
      stringstream msg;
      // Send starting message for the start of a game
      if (round == 1) {
         while (users[getEnemyIndex(player)].compare("") == 0) {
            // Wait until both player usernames appear
         }
         msg << "\n" << users[player.getID()] << " vs " << users[getEnemyIndex(player)] << "!\n";
      }

      msg << "\nRound " << round++ << endl;           // Message for each round
      sendMsg(player, msg.str());                     // Send the message to the player
      waitForAnswers(player);                         // Waits answers from player and enemy 
      determineWinner(player);                        // Determines the winner for current round
      score = scoreboard[player.getID()];             // Gets score
      enemyScore = scoreboard[getEnemyIndex(player)]; // Gets the opponent's score
   }

   if (!player.isGuest()) {
      // Critical section

      // Will make threads wait different amounts of time depending  on the number of messages
      // being sent at the same time to prevent messages being corrupted
      ++messageTimer;
      this_thread::sleep_for(chrono::microseconds(messageTimer*100));

      // This will lock all threads while one thread is sending a message
      while (threadLock) {
         // Each thread wait for different times so they dont all get released at once
         this_thread::sleep_for(chrono::microseconds(messageTimer*100));
      }

      // Sets the threadLock so other threads will wait
      threadLock = true;

      // Access database to set player's statistics
      data.setStats(users[player.getID()], win[player.getID()], draws[player.getID()]);

      // Resets the threadLock, notifying other threads
      threadLock = false;
      --messageTimer;
      // End of critical section
   }

   // Removes the player from the roster and scoreboard
   cout << "[Game] " << users[player.getID()] << " (" << player.getID() << ") has left" << endl;
   roster[player.getID()] = false;
   users[player.getID()] = "";
   answers[player.getID()] = "0";
   draws[player.getID()] = 0;
   win[player.getID()] = false;

   while (roster[getEnemyIndex(player)] != false) {
      // Wait for opponent to exit before resetting scoreboard 
   }
   scoreboard[player.getID()] = 0;
   if (player.getID() % 2 == 0) {
      this_thread::sleep_for(chrono::microseconds(1000000));
   }
   player.setID(0); // Set players id to 0 for inactive
}


/**
 * @brief Creates and sends the welcome message to the player.
 * @param player A reference to the player passed from startMenu function.
*/
void Server::welcomeMessage(Player &player) {
   string msg = "\n---------------- Welcome to Roshambo ----------------\n\n";
   sendMsg(player, msg);
}


/**
 * @brief Creates and sends the menu message to the player.
 * @param player A reference to the player passed from startMenu function.
*/
void Server::menuMessage(Player &player) {
   stringstream msg;

   msg << "Main Menu:\n"
       << "1: View the rules\n"
       << "2: View the stats of an existing player\n"
       << "3: View the leaderboard\n"
       << "4: Register as a new player\n"
       << "5: Log in as an existing player\n"
       << "6: Play as a guest\n"
       << "7: Exit Game\n" << endl;
   string temp(msg.str());
   sendMsg(player, temp);
}


/**
 * @brief Creates and sends the message for the rules of the game to the player.
 * @param player A reference to the player passed from startMenu function.
*/
void Server::displayRules(Player &player) { 
   stringstream msg;

   cout << "[Rule] Displaying rules" << endl;
   msg << "\n~~~ Rules ~~~\n\n" <<
   "Each player will pick either rock, paper, or scissors." <<
   "\n - Rock breaks Scissors\n - Scissors cuts Paper\n - Paper covers Rock\n\n" << endl;
   string temp = msg.str();
   sendMsg(player, temp);
}

/**
 * @brief Creates and sends the stats of a player from prompt to the player.
 * @param player A reference to the player created in startMenu function.
 * @param data A reference to the database created in startMenu function.
 * @return Returns a code based on function success.
*/
int Server::displayStat(Player &player, Data &data) {
   string ans; // Message for the player 
   string name; // Name to search for
   string msg = "Type the name for the player's stats you want to view: ";
   sendMsg(player, msg);
   recvMsg(player);
   name = buffer; // Store response

   cout << "[Stat] Searching for " << name << "..." << endl;
   int resp = data.getStats(ans, name); // Search for specified player
   if (resp == 1) { // If successful
      cout << "[Stat] Displaying stats for " << name << endl;
      sendMsg(player, "\n" + ans + "\n\n");
   }
   this_thread::sleep_for(chrono::microseconds(1000000));
   return resp;
}


/**
 * @brief Creates and sends the top players leaderboard to the player.
 * @param player A reference to the player created in startMenu function.
 * @param data A reference to the database created in startMenu function.
 * @return Returns a code based on function success.
*/
int Server::displayBoard(Player &player, Data &data) {
   string ans = "\n~~~ Roshambo Leaderboard! ~~~\n\n";
   int resp = data.getBoard(ans);
   if (resp == 1) { // If successful
      cout << "[Lead] Displaying leaderboard" << endl;
      sendMsg(player, ans + "\n\n");
   }
   this_thread::sleep_for(chrono::microseconds(1000000));
   return resp;
}


/**
 * @brief Creates a new database entry using a prompt send to the player.
 * @param player A reference to the player created in startMenu function.
 * @param data A reference to the database created in startMenu function.
 * @return Returns a code based on function success.
*/
int Server::regPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Type in your username (alphanumeric only, max 20 chars.): ");
   recvMsg(player);
   name = buffer; // Store response

   // Check if username is already playing a game
   for (int i = 0; i < numOfPlayers; i++) {
      if (users[i].compare(name) == 0) {
         return -1;
      }
   }

   cout << "[Regi] Searching for " << name << "..." << endl;

   // Critical section
   ++messageTimer;
   this_thread::sleep_for(chrono::microseconds(messageTimer*100));

   while (threadLock) {
      this_thread::sleep_for(chrono::microseconds(messageTimer*100));
   }

   threadLock = true;

   // Access database to register new player
   int resp = data.regUser(name);

   threadLock = false;
   --messageTimer;
   // End of critical section

   if (resp == 1) { // If successful
      cout << "[Regi] Registering " << name << endl;
      sendMsg(player, "Welcome to Roshambo, " + name + "!\n\n");
      player.setName(name.c_str());
   }

   this_thread::sleep_for(chrono::microseconds(1000000));
   return resp;
}


/**
 * @brief Checks for a database entry using a prompt send to the player.
 * @param player A reference to the player created in startMenu function.
 * @param data A reference to the database created in startMenu function.
 * @return Returns a code based on function success.
*/
int Server::logPlayer(Player &player, Data &data) {
   string name;

   sendMsg(player, "Username: ");
   recvMsg(player);

   name = buffer;

   for (int i = 0; i < numOfPlayers; i++) {
      if (users[i].compare(name) == 0) {
         return -1;
      }
   }

   cout << "[Logg] Searching for " << name << "..." << endl;
   
   // Critical section
   ++messageTimer;
   this_thread::sleep_for(chrono::microseconds(messageTimer*100));

   while (threadLock) {
      this_thread::sleep_for(chrono::microseconds(messageTimer*100));
   }

   threadLock = true;

   // Access database to log player
   int resp = data.logUser(name);

   threadLock = false;
   --messageTimer;
   // End of critical section

   if (resp == 1) {
      cout << "[Logg] Logging " << name << endl;
      sendMsg(player, "Welcome back, " + name + "!\n");
      player.setName(name.c_str());
   }

   return resp;
}

/**
 * @brief Assigns the player an ID for indexing in global variables. The player will be assigned the
 * index based on available slots in roster. If a player is assigned to a slot and it does not get
 * an enemy in 15 secs it will reset their ID and remove them from the roster and search for a new
 * ID in roster. If the enemy shows up before 15 secs it will break and both players will start the
 * match.
 * @param player A reference to the player created in startMenu function.
*/
void Server::assignPlayerID(Player &player) {
   while (true) {
      // Search roster for open slot
      for (int i = 1; i < numOfPlayers; i++) {
         if (roster[i] == false) {
            roster[i] = true;
            player.setID(i);
            break;
         }
      }

      // Start timer for thread to possibly reset and search again
      int timer = 0;
      while (roster[getEnemyIndex(player)] == false) {
         // Wait for opponent
         this_thread::sleep_for(chrono::microseconds(1000000));
         ++timer;
         if (timer >= 15) {
            roster[player.getID()] = false;
            player.setID(0);
            break;
         }
      }

      // If player ID is 0 then no opponent found restart search
      if (player.getID() == 0) {
         continue;
      }
      // Else opponent found leave function
      else {
         return;
      }
   }
}


/**
 * @brief Recieves the answers from each player and makes them wait for their opponent to also
 * answer before proceeding to determine the winners.
 * @param player A reference to the player created in startMenu function.
*/
void Server::waitForAnswers(Player &player) {
   recvMsg(player);           // Wait for players to input answer
   player.setChoice(buffer);  // Sets players choice
   cout << "[Game] " << users[player.getID()] << " (" << player.getID() << ")'s choice was: "
        << player.getChoice() << endl;
   answers[player.getID()] = player.getChoice();
   while (answers[getEnemyIndex(player)].compare("0") == 0) {
      // Wait for opponent to answer
   }
}


/**
 * @brief Determines the winner between current players in match based on rps logic. Then sends the
 * corresponding win/lose message to each player. If there is a winner both players will recieve the
 * terminating message and client.cpp will end bestOutOfThree loop.
 * @param player A reference to the player created in startMenu function.
*/
void Server::determineWinner(Player &player) {
   stringstream msg;                            // The message sent for showing the winner
   string p1 = answers[player.getID()];         // The current players answer
   string p2 = answers[getEnemyIndex(player)];  // The current players enemy answer

   // if player disconnects mid game
   if (p2.compare("") == 0) {
      cout << "[Game] Player " << users[player.getID()] << " (" << player.getID()
           << ") has disconnected" << endl;
      msg << users[getEnemyIndex(player)] << " Disconnected...\n\n You Win the Match!\n\nExit";
      sendMsg(player, msg.str());

      roster[getEnemyIndex(player)] = false; // Reset enemies roster
      users[getEnemyIndex(player)] = "";     // Reset enemies username
      answers[player.getID()] = "0";         // Reset answers for player
      answers[getEnemyIndex(player)] = "0";  // Reset enemies answers
      scoreboard[player.getID()] = 2;        // Increase scoreboard to break loop in startGame
      scoreboard[getEnemyIndex(player)] = 0; // Reset disconected players scoreboard
      return;
   }

   msg << "\n\n" << drawChoice(p1) << "\n\n    vs    \n\n" << drawChoice(p2) << "\n\n";

   if (p1.compare("rock") == 0) {               // Player 1 picks rock
      if (p2.compare("paper") == 0) {           // Player 2 picks paper
         msg << "You Lost\n\nPaper covers Rock!";
      }
      else if (p2.compare("scissors") == 0) {   // Player 2 picks scissors
         msg << "You Won\n\nRock smashes Scissors!";
         scoreboard[player.getID()]++;          // Player 1 won, increase scoreboard
      }
      else {
         // Draw
         msg << "Draw!";
         draws[player.getID()]++;
      }
   }
   // player 1 picks paper
   else if (p1.compare("paper") == 0) {
      // player 2 picks Scissors
      if (p2.compare("scissors") == 0) {
         msg << "You Lost\n\nScissors cuts Paper!";
      }
      else if (p2.compare("rock") == 0) {    // Player 2 picks Rock
         msg << "You Won\n\nPaper covers Rock!";
         scoreboard[player.getID()]++;       // Player 1 won, increase scoreboard
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         draws[player.getID()]++;
      }
   }
   // player 1 picks Scissors
   else {
      // player 2 picks Rock
      if (p2.compare("rock") == 0) {
         msg << "You Lost\n\nRock smashes Scissors!";
      }
      else if (p2.compare("paper") == 0) {   // Player 2 picks Paper
         msg << "You Won\n\nScissors cuts Paper!";
         scoreboard[player.getID()]++;       // Player 1 won, increase scoreboard
      }
      // opponent picks Paper
      else {
         // Draw
         msg << "Draw!";
         draws[player.getID()]++;
      }
   }

   // Sleep thread 1 or 2 doesnt matter
   // This will allow them both to catch up prior to determining the score
   this_thread::sleep_for(chrono::microseconds(100));

   int score = scoreboard[player.getID()];  // Gets current players score
   int enemyScore = scoreboard[getEnemyIndex(player)]; // Gets the current players enemy score

   msg << "\n\nYour score: " << score
       << "\n" << users[getEnemyIndex(player)] <<  "'s score: " << enemyScore << "\n\n";

   // If either has a score of 2 then they won
   if (score == 2 || enemyScore == 2) {
      if (score == 2) { // If its the current player
         msg << "You Won the Match!\n\n";
         win[player.getID()] = true;
      }
      else { // If its the enemy
         msg << "You Lost the Match!\n\n";
      }
      msg << "Exit"; // Terminating message for client.cpp
   }
   sendMsg(player, msg.str());
   answers[player.getID()] = "0"; // Reset answers for each player
}


/**
 * @brief Gets the current players enemy index.
 * @param player A reference to the player created in startMenu function.
 * @return Returns the index of the current players enemy index.
*/
int Server::getEnemyIndex(Player& player) const {
   // If the current player ID is 1 then its enemy is 2
   // If the current player ID is 2 then its enemy is 1
   // This applies to 3 and 4, and so on
   return player.getID() % 2 == 0 ? player.getID() - 1 : player.getID() + 1;
}


/**
 * @brief Draws an ASCII hand signal for rock, paper, or scissors.
 * @param choice Rock, paper, scissors.
 * @return Returns a text based image of rock, paper, scissors.
*/
string Server::drawChoice(const string choice) const {
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
   else { // Scissors
      return "    _______\n"
             "---'   ____)____\n"
             "          ______)\n"
             "       __________)\n"
             "      (____)\n"
             "---.__(___)\n";
   }

}


/**
 * @brief Sends a message to the player object.
 * @param player A reference to the player created in startMenu function.
 * @param msg Returns the message that is being sent to the client.
*/
void Server::sendMsg(Player &player, string msg) {

   // Critical section
   ++messageTimer;
   this_thread::sleep_for(chrono::microseconds(messageTimer*100));
   
   while (threadLock) {
      this_thread::sleep_for(chrono::microseconds(messageTimer*100));
   }

   threadLock = true;

   // Sends the actual message
   memset(&buffer, 0, sizeof(buffer));
   strcpy(buffer, msg.c_str());
   send(player.getSD(), buffer, sizeof(buffer), 0);

   threadLock = false;
   --messageTimer;
   // End of critical section
}


/**
 * @brief Recieves a message from the players socket descriptor.
 * @param player A reference to the player created in startMenu function.
*/
void Server::recvMsg(Player &player) {
   memset(&buffer, 0, sizeof(buffer));
   recv(player.getSD(), buffer, sizeof(buffer) , 0);
}


/**
 * @brief Selects an error messaged based on the error code received.
 * @param code Error code.
 * @return Returns a string based on code received.
*/
string Server::displayErr(const int code) const {
   stringstream msg;
   msg << "Error: ";
   switch(code) {
      case -3:
         msg << "Not a valid name...\n\n";
         break;
      case -2:
         msg << "Name not found...\n\n";
         break;
      case -1:
         msg << "Name already taken...\n\n";
         break;
      case 0:
         msg << "Failed to open leaderboard...\n\n";
         break;
      default:
         msg << "Code not found (" << code << ")\n\n";
         break;
   }
   return msg.str();
}