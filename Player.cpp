// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Player.h"


Player::Player() {}

Player::Player(const int sd) {
   socketDescriptor = sd;
   guest = false;
}

Player::~Player() {}

int Player::getSD() const {
   return socketDescriptor;
}

void Player::setSD(const int sd) {
   socketDescriptor = sd;
}

int Player::getID() const {
   return playerNumber;
}

void Player::setID(const int ID) {
   playerNumber = ID;
}

const char* Player::getName() const {
   return playerName;
}

void Player::setName(const char* name) {
   playerName = name;
}

char* Player::getChoice() const {
   return choice;
}

void Player::setChoice(char* ans) {
   choice = ans;
}

void Player::setGuest() {
   guest = true;
}

bool Player::isGuest() const {
   return guest;
}