#ifndef PLAYER_H
#define PLAYER_H


class Player {
   public:
      Player();
      Player(int sd, int num);
      ~Player();
      int getSD();
      void setSD(int sd);
      int getID();
      void setID(int ID);
      const char* getName();
      void setName(const char* name);
      char* getChoice();
      void setChoice(char* ans);
      int getRound();
      void setRound(int rnd);
      int getMatch();
      void setMatch();
      int getDraw();
      void setDraw();
      void setGuest();
      bool isGuest();
   
   private:
      int socketDescriptor;
      int playerNumber;
      const char* playerName;
      char* choice;
      int round;
      int match;
      int draw;
      bool guest;
};

#endif