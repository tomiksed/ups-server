//
// Created by justme on 19.12.16.
//

#include "Player.h"

Player::Player() {
    this->socket = -1;

    this->inGame = false;
    this->availible = false;

    this->joiningPlayer = nullptr;
    this->askedForJoiningBy = nullptr;

    this->lost = false;
}

Player::Player(int socket) {
    this->socket = socket;
    this->socketSet = true;

    this->inGame = false;
    this->availible = false;

    this->joiningPlayer = nullptr;
    this->askedForJoiningBy = nullptr;

    this->lost = false;
}

Player::Player(int socket, std::string name) {
    this->socket = socket;
    this->socketSet = true;
    this->name = name;
    this->nameSet = true;

    this->inGame = false;
    this->availible = false;

    this->joiningPlayer = nullptr;
    this->askedForJoiningBy = nullptr;

    this->lost = false;
}

bool Player::hasSocket() {
    return this->socketSet;
}

bool Player::hasName() {
    return this->nameSet;
}

void Player::removeSocket() {
    this->socket = -1;
    this->socketSet = false;
}

std::string Player::getName() {
    return name;
}

void Player::setName(std::string name) {
    this->name = name;
    this->nameSet = true;
}

int Player::getSocket() {
    return socket;
}

void Player::setSocket(int socket) {
    this->socket = socket;
    this->socketSet = true;
}

bool Player::isInGame() {
    return this->inGame;
}

void Player::setInGame(bool inGame) {
    this->inGame = inGame;
}

bool Player::isAvailible() {
    return this->availible;
}

void Player::setAvailible(bool availible) {
    this->availible = availible;
}
