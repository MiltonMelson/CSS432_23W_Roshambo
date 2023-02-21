#include "Player.h"

Player::Player() {}

Player::Player(int sd, int num) {
   socketDescriptor = sd;
   playerNumber = num;
   online = true;
}

Player::~Player() {}

int Player::getSD() {
   return socketDescriptor;
}

void Player::setSD(int sd) {
   socketDescriptor = sd;
}

int Player::getID() {
   return playerNumber;
}

void Player::setID(int ID) {
   playerNumber = ID;
}

char* Player::getChoice() {
   return choice;
}

void Player::setChoice(char* ans) {
   choice = ans;
}

void Player::offline() {
   online = false;
}
bool Player::isDisconnected() {
   return online;
}