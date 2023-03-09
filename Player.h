// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#ifndef PLAYER_H
#define PLAYER_H

class Player {
   public:
      Player();
      Player(const int sd);
      ~Player();
      int getSD() const;
      void setSD(const int sd);
      int getID() const;
      void setID(const int ID);
      const char* getName() const;
      void setName(const char* name);
      char* getChoice() const;
      void setChoice(char* ans);
      void setGuest();
      bool isGuest() const;
   
   private:
      int socketDescriptor;   // Socket descriptor so the server can send messages to each player
      int playerNumber;       // Assigned temporarily based on number of players in active match (used for indexing)
      const char* playerName; // The player's name
      char* choice;           // The choice made by the player
      bool guest;             // Is true if the player is a guest
};

#endif