#ifndef PLAYER_H
#define PLAYER_H

class Player {
   public:
      int socketDescriptor;
      int playerNumber;
      char* choice;

      Player();
      Player(int sd, int num);
      ~Player();
   
};

#endif