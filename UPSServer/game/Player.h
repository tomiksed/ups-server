//
// Created by justme on 19.12.16.
//

#ifndef UPSSERVER_PLAYER_H
#define UPSSERVER_PLAYER_H

#include <string>

class Player {
public:

    Player();
    Player(int socket);
    Player(int socket, std::string name);

    bool hasSocket();
    bool hasName();

    void removeSocket();

    std::string getName();

    void setName(std::string name);

    int getSocket();

    void setSocket(int socket);

    bool isInGame();
    void setInGame(bool inGame);

    bool isAvailible();
    void setAvailible(bool availible);

    Player *joiningPlayer;
    Player *askedForJoiningBy;

private:

    bool nameSet;
    std::string name;

    bool socketSet;
    int socket;

    bool inGame;
    bool availible;
};


#endif //UPSSERVER_PLAYER_H
