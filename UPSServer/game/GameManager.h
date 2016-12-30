//
// Created by justme on 30.12.16.
//

#ifndef UPSSERVER_GAMEMANAGER_H
#define UPSSERVER_GAMEMANAGER_H


#include <vector>
#include <mutex>
#include "Game.h"

class GameManager {
public:
    static GameManager *instance();

    Game *getGameByPlayer(Player *p);

    Player *getPlayersOponent(Player *p);

    Game *registerGame(Player *p1, Player *p2);

    bool isGameWon(Game *game, int moveI, int moveJ);

private:
    GameManager();

    std::mutex gameMutex;

    static int maxGameId;

    static GameManager *INSTANCE;

    std::vector<Game *> *games;
};


#endif //UPSSERVER_GAMEMANAGER_H
