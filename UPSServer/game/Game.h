//
// Created by justme on 30.12.16.
//

#ifndef UPSSERVER_GAME_H
#define UPSSERVER_GAME_H


#include "Player.h"

class Game {
public:

    Game(int id);

    int field[8][8];

    Player *playerOnTurn;


    Player *getPlayer1();
    Player *getPlayer2();

    void setPlayer1(Player *nP);
    void setPlayer2(Player *nP);

    int getId() { return this->id; }

    std::string *getGameString();

private:

    int id;

    Player *p1;
    Player *p2;
};


#endif //UPSSERVER_GAME_H
