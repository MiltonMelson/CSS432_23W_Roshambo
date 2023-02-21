#include "Player.h"

Player::Player() {}
Player::Player(int sd, int num) {
   socketDescriptor = sd;
   playerNumber = num;
}
Player::~Player() {}