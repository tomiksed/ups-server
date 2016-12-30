//
// Created by justme on 30.12.16.
//

#include "GameManager.h"

GameManager *GameManager::INSTANCE = new GameManager();
int GameManager::maxGameId = 0;

GameManager *GameManager::instance() {
    return GameManager::INSTANCE;
}

GameManager::GameManager() {
    this->games = new std::vector<Game *>();
}

Game *GameManager::getGameByPlayer(Player *p) {
    gameMutex.lock();

        Game *game = nullptr;
        for (Game *g : *this->games) {
            if (g->getPlayer1()->getName() == p->getName() || g->getPlayer2()->getName() == p->getName()) {
                game = g;
            }
        }

    gameMutex.unlock();

    return game;
}

Player *GameManager::getPlayersOponent(Player *p) {
    Game *pG = this->getGameByPlayer(p);

    if (pG == nullptr) {
        return nullptr;
    }

    if (pG->getPlayer1()->getName() == p->getName()) {
        return pG->getPlayer2();
    } else {
        return pG->getPlayer1();
    }
}

Game *GameManager::registerGame(Player *p1, Player *p2) {
    gameMutex.lock();

        Game *g = new Game(maxGameId);
        maxGameId++;

        g->setPlayer1(p1);
        g->setPlayer2(p2);

        this->games->push_back(g);

    gameMutex.unlock();

    return g;
}

bool controlBlack(Game *game, bool ctrlField[8][8], int i, int j) {
    if (i < 0 || i > 7 || j < 0 || j > 7) return false;
    if (game->field[i][j] == 0) return false;

    if (ctrlField[i][j] == true) {
        return true;
    } else {
        ctrlField[i][j] = true;
    }

    switch (game->field[i][j]) {
        case 1: {
            return controlBlack(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i + 1, j);
            break;
        }
        case 2: {
            return controlBlack(game, ctrlField, i, j - 1) || controlBlack(game, ctrlField, i + 1, j);
            break;
        }
        case 3: {
            return controlBlack(game, ctrlField, i + 1, j) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
        case 4: {
            return controlBlack(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
        case 5: {
            return controlBlack(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i, j - 1);
            break;
        }
        case 6: {
            return controlBlack(game, ctrlField, i + 1, j) || controlBlack(game, ctrlField, i, j - 1);
            break;
        }
    }
}

bool controlWhite(Game *game, bool ctrlField[8][8], int i, int j) {
    if (i < 0 || i > 7 || j < 0 || j > 7) return false;
    if (game->field[i][j] == 0) return false;

    if (ctrlField[i][j] == true) {
        return true;
    } else {
        ctrlField[i][j] = true;
    }

    switch (game->field[i][j]) {
        case 1: {
            return controlWhite(game, ctrlField, i, j - 1) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
        case 2: {
            return controlWhite(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
        case 3: {
            return controlWhite(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i, j - 1);
            break;
        }
        case 4: {
            return controlWhite(game, ctrlField, i + 1, j) || controlBlack(game, ctrlField, i, j - 1);
            break;
        }
        case 5: {
            return controlWhite(game, ctrlField, i + 1, j) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
        case 6: {
            return controlWhite(game, ctrlField, i - 1, j) || controlBlack(game, ctrlField, i, j + 1);
            break;
        }
    }
}

bool GameManager::isGameWon(Game *game, int moveI, int moveJ) {
    bool ctrlField[8][8];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ctrlField[i][j] = false;
        }
    }
    ctrlField[moveI][moveJ] = true;

    if (controlBlack(game, ctrlField, moveI, moveJ)) {
        return true;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ctrlField[i][j] = false;
        }
    }
    ctrlField[moveI][moveJ] = true;

    if (controlWhite(game, ctrlField, moveI, moveJ)) {
        return true;
    }

    return false;
}
