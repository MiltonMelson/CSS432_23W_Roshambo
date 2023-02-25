#include "Player.h"

Player::Player() {}

Player::Player(int sd, int num) {
   socketDescriptor = sd;
   playerNumber = num;
   online = true;
   guest = false;
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

char* Player::getName() {
   return playerName;
}

void Player::setName(char* name) {
   playerName = name;
}

char* Player::getChoice() {
   return choice;
}

void Player::setChoice(char* ans) {
   choice = ans;
}

int Player::getRound() {
   return round;
}

void Player::setRound(int rnd) {
   round += rnd;
}

int Player::getMatch() {
   return match;
}

void Player::setMatch() {
   match++;
}

int Player::getDraw() {
   return draw;
}

void Player::setDraw() {
   draw++;
}

void Player::setGuest() {
   guest = true;
}

bool Player::isGuest() {
   return guest;
}

void Player::offline() {
   online = false;
}

bool Player::isDisconnected() {
   return online;
}