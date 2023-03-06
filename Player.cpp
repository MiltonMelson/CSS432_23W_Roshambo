// CSS432 Winter 2023
// Professor: Yang Peng
// Students: Milton Melson, Oliver Fernandez
// Project Name: Roshambo

#include "Player.h"

Player::Player() {}

Player::Player(int sd) {
   socketDescriptor = sd;
   guest = false;
   ready = false;
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

const char* Player::getName() {
   return playerName;
}

void Player::setName(const char* name) {
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

bool Player::isReady() {
   return ready;
}

void Player::makeReady() {
   ready = true;
}

void Player::notReady() {
   ready = false;
}