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

void GameManager::deleteGame(Game *game) {
    gameMutex.lock();

        int index = -1;

        for (Game *g : *this->games) {
            index++;
            if (g->getId() == game->getId()) {
                break;
            }
        }

        this->games->erase(this->games->begin() + index);

    gameMutex.unlock();
}

bool controlBlack(Game *game, bool ctrlField[8][8], int i, int j, int fromI, int fromJ) {
    if (i < 0 || i > 7 || j < 0 || j > 7) return false;
    if (game->field[i][j] == 0) return false;

    if (ctrlField[i][j] == true) {
        return true;
    } else {
        ctrlField[i][j] = true;
    }

    switch (game->field[i][j]) {
        case 1: {
            if ((fromI != i - 1 || fromJ != j) && controlBlack(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i + 1 || fromJ != j) && controlBlack(game, ctrlField, i + 1, j, i, j)) return true;
            break;
        }
        case 2: {
            if ((fromI != i || fromJ != j - 1) && controlBlack(game, ctrlField, i, j - 1, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlBlack(game, ctrlField, i, j + 1, i, j)) return true;
            break;
        }
        case 3: {
            if ((fromI != i + 1 || fromJ != j) && controlBlack(game, ctrlField, i + 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlBlack(game, ctrlField, i, j + 1, i, j)) return true;
            break;
        }
        case 4: {
            if ((fromI != i - 1 || fromJ != j) && controlBlack(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlBlack(game, ctrlField, i, j + 1, i, j)) return true;
            break;
        }
        case 5: {
            if ((fromI != i - 1 || fromJ != j) && controlBlack(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j - 1) && controlBlack(game, ctrlField, i, j - 1, i, j)) return true;
            break;
        }
        case 6: {
            if ((fromI != i + 1 || fromJ != j) && controlBlack(game, ctrlField, i + 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j - 1) && controlBlack(game, ctrlField, i, j - 1, i, j)) return true;
            break;
        }
    }
}

bool controlWhite(Game *game, bool ctrlField[8][8], int i, int j, int fromI, int fromJ) {
    if (i < 0 || i > 7 || j < 0 || j > 7) return false;
    if (game->field[i][j] == 0) return false;

    if (ctrlField[i][j] == true) {
        return true;
    } else {
        ctrlField[i][j] = true;
    }

    switch (game->field[i][j]) {
        case 1: {
            if ((fromI != i || fromJ != j - 1) && controlWhite(game, ctrlField, i, j - 1, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlWhite(game, ctrlField, i, j + 1, i, j)) return true;
            break;
        }
        case 2: {
            if ((fromI != i - 1 || fromJ != j) && controlWhite(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i + 1 || fromJ != j) && controlWhite(game, ctrlField, i + 1, j, i, j)) return true;
            break;
        }
        case 3: {
            if ((fromI != i - 1 || fromJ != j) && controlWhite(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j - 1) && controlWhite(game, ctrlField, i, j - 1, i, j)) return true;
            break;
        }
        case 4: {
            if ((fromI != i + 1 || fromJ != j) && controlWhite(game, ctrlField, i + 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j - 1) && controlWhite(game, ctrlField, i, j - 1, i, j)) return true;
            break;
        }
        case 5: {
            if ((fromI != i + 1 || fromJ != j) && controlWhite(game, ctrlField, i + 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlWhite(game, ctrlField, i, j + 1, i, j)) return true;
            break;
        }
        case 6: {
            if ((fromI != i - 1 || fromJ != j) && controlWhite(game, ctrlField, i - 1, j, i, j)) return true;
            if ((fromI != i || fromJ != j + 1) && controlWhite(game, ctrlField, i, j + 1, i, j)) return true;
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

    if (controlWhite(game, ctrlField, moveI, moveJ, moveI, moveJ)) {
        return true;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ctrlField[i][j] = false;
        }
    }

    if (controlBlack(game, ctrlField, moveI, moveJ, moveI, moveJ)) {
        return true;
    }

    return false;
}
