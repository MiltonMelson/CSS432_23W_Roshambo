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
      char* getChoice();
      void setChoice(char* ans);
      void offline();
      bool isDisconnected();
   
   private:
      int socketDescriptor;
      int playerNumber;
      char* choice;
      bool online;
};

#endif