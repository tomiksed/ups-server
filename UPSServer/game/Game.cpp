#include "Game.h"

Game::Game(int id) {
    this->id = id;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            field[i][j] = 0;
        }
    }
}

Player *Game::getPlayer1() {
    return this->p1;
}

Player *Game::getPlayer2() {
    return this->p2;
}

void Game::setPlayer1(Player *nP) {
    this->p1 = nP;
}

void Game::setPlayer2(Player *nP) {
    this->p2 = nP;
}

std::string *Game::getGameString() {
    std::string *gS = new std::string("");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (gS->size() != 0) {
                gS->append(";");
            }
            gS->append(std::to_string(this->field[i][j]));
        }
    }

    return gS;
}