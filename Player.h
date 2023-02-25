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
      char* getName();
      void setName(char* name);
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
      void offline();
      bool isDisconnected();
   
   private:
      int socketDescriptor;
      int playerNumber;
      char* playerName;
      char* choice;
      int round;
      int match;
      int draw;
      bool online;
      bool guest;
};

#endif