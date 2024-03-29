#include "CommandManager.h"
#include "packet.h"
#include "Sender.h"
#include "../game/Game.h"
#include "../game/GameManager.h"

CommandManager *CommandManager::INSTANCE = new CommandManager();

CommandManager::CommandManager(){}

CommandManager *CommandManager::instance(){
    return INSTANCE;
}

void CommandManager::init() {
    this->commandQueue = new ConcurrentQueue<Message *>();
}

std::thread *CommandManager::start() {
    LOG(log::DEBUG, std::string("Command manager started"));
    this->cmThread = new std::thread(&CommandManager::run, this);
}

bool isUsernameAvailible(std::string username) {
    for (Player *p : *Server::players) {
        if (p->getName() == username) {
            return false;
        }
    }
    return true;
}

/*
 * Proceeds new player connection. Checks if the player name is not in use.
 */
void proceedNewPlayerConnection(Message *mess) {
    std::string username = *((std::string *) mess->getPayload()->at(0));

    if (isUsernameAvailible(username)) {
        mess->player->setName(username);
        mess->player->setAvailible(true);

        Message *ackMess = new Message(H_S_ACK, (*Serializer::instance()->headToFormatMap)[H_S_ACK]);
        ackMess->player = mess->player;
        Sender::instance()->registerMessage(ackMess);
    } else {
        Message *unMess = new Message(H_S_NACK_REAS, (*Serializer::instance()->headToFormatMap)[H_S_NACK_REAS]);
        unMess->addData(new std::string("Username already in use!"));
        unMess->player = mess->player;
        Sender::instance()->registerMessage(unMess);
    }
}

void proceedPlayerListRequest(Message *mess) {
    std::string *result = new std::string("");
    uint32_t *resultSize = new uint32_t;
    *resultSize = 0;

    for (Player *p : *Server::instance()->players) {
        if (!p->isInGame() && p->isAvailible() && p->hasSocket() && p->hasName() && p->getName() != mess->player->getName()) {
            if (*resultSize > 0) result->append(";");
            result->append(p->getName());
            (*resultSize)++;
        }
    }

    Message *answer = new Message(H_S_PL_LIST_ANS, (*Serializer::instance()->headToFormatMap)[H_S_PL_LIST_ANS]);
    answer->addData((void *) resultSize);
    answer->addData((void *) result);
    answer->player = mess->player;

    Sender::instance()->registerMessage(answer);
}

/*
 * Proceeds player reconnection. Checks if the player is realy lost.
 */
void proceedPlayerReconnection(Message *mess) {
    Player *newP = mess->player;

    Player *oldP = Server::instance()->getPlayerByName(*((std::string *)mess->getPayload()->at(0)));


    if (oldP == nullptr || !oldP->lost) { /* Bullshit, doesn't exist or isn't lost */
        Message *ans = new Message(H_S_NACK, (*Serializer::instance()->headToFormatMap)[H_S_NACK]);
        ans->player = newP;
        Sender::instance()->registerMessage(ans);
    } else { /**/
        newP->setName(oldP->getName());
        newP->setAvailible(false);
        newP->setInGame(true);

        Game *game = GameManager::instance()->getGameByPlayer(oldP);

        if (game->getPlayer1()->getName() == newP->getName()) {
            game->setPlayer1(newP);
        } else {
            game->setPlayer2(newP);
        }

        /* Inform oponent and send them complete game info */
        Player *oponent = GameManager::instance()->getPlayersOponent(newP);

        Message *gameCon1 = new Message(H_S_GAME_CON, (*Serializer::instance()->headToFormatMap)[H_S_GAME_CON]);
        gameCon1->addData(new std::string(game->playerOnTurn->getName()));
        gameCon1->addData(new std::string(oponent->getName()));
        gameCon1->addData(game->getGameString());
        gameCon1->player = newP;

        Message *gameCon2 = new Message(H_S_GAME_CON, (*Serializer::instance()->headToFormatMap)[H_S_GAME_CON]);
        gameCon2->addData(new std::string(game->playerOnTurn->getName()));
        gameCon2->addData(new std::string(newP->getName()));
        gameCon2->addData(game->getGameString());
        gameCon2->player = oponent;

        Sender::instance()->registerMessage(gameCon1);
        Sender::instance()->registerMessage(gameCon2);

        Server::instance()->deletePlayer(oldP);
    }
}

void proceedPlayerJoining(Message *mess) {
    Player *requesting = mess->player;
    Player *oponent = Server::instance()->getPlayerByName(*((std::string *) mess->getPayload()->at(0)));

    if (oponent != nullptr && oponent->isAvailible() && oponent->hasSocket() && !oponent->isInGame()) { /* Is joinable */
        /* Send ACK to requesting player */
        Message *ack = new Message(H_S_ACK, (*Serializer::instance()->headToFormatMap)[H_S_ACK]);
        ack->player = requesting;
        Sender::instance()->registerMessage(ack);
        /* Make him not availible */
        requesting->setAvailible(false);
        /* Set him the joining player */
        requesting->joiningPlayer = oponent;

        /* Send request to player to be joined */
        Message *joinRequest = new Message(H_S_JOIN_PLAYER, (*Serializer::instance()->headToFormatMap)[H_S_JOIN_PLAYER]);
        joinRequest->addData(new std::string(requesting->getName()));
        joinRequest->player = oponent;
        Sender::instance()->registerMessage(joinRequest);
        /* Set him the player that asked for joining - to be easily found */
        oponent->askedForJoiningBy = requesting;

        requesting->setAvailible(false);
        oponent->setAvailible(false);
    } else { /* Isn't joinable */
        Message *nack = new Message(H_S_NACK, (*Serializer::instance()->headToFormatMap)[H_S_NACK]);
        nack->player = requesting;
        Sender::instance()->registerMessage(nack);
    }
}

void proceedAcceptJoin(Message *mess) {
    Player *asking = mess->player->askedForJoiningBy;
    Player *asked = mess->player;

    mess->player->askedForJoiningBy = nullptr;
    mess->player->joiningPlayer = nullptr;

    asking->setAvailible(false);
    asked->setAvailible(false);

    if (asking->joiningPlayer != nullptr) { /* They can start the game */

        LOG_INFO("Starting game between " + asking->getName() + " and " + asked->getName());

        /* Put them in a game */
        Game *game = GameManager::instance()->registerGame(asking, asked);
        asking->setInGame(true);
        asked->setInGame(true);
        game->playerOnTurn = asking;

        /* Send them a game start message! */
        Message *p1GameInfo = new Message(H_S_GAME_START, (*Serializer::instance()->headToFormatMap)[H_S_GAME_START]);
        p1GameInfo->addData(new std::string(asking->getName()));
        p1GameInfo->addData(new std::string(asked->getName()));
        p1GameInfo->player = asking;

        Message *p2GameInfo = new Message(H_S_GAME_START, (*Serializer::instance()->headToFormatMap)[H_S_GAME_START]);
        p2GameInfo->addData(new std::string(asking->getName()));
        p2GameInfo->addData(new std::string(asking->getName()));
        p2GameInfo->player = asked;

        Sender::instance()->registerMessage(p1GameInfo);
        Sender::instance()->registerMessage(p2GameInfo);
    } else {
        /* Asking player disconnected, so inform asked */
        if (asked->hasSocket()) {
            Message *ans = new Message(H_S_NACK_REAS, (*Serializer::instance()->headToFormatMap)[H_S_NACK_REAS]);
            ans->addData(new std::string("Player who asked is gone"));
            ans->player = asked;
            Sender::instance()->registerMessage(ans);
        }
    }
}

void proceedDeclineJoin(Message *mess) {
    Player *asking = mess->player->askedForJoiningBy;
    Player *asked = mess->player;

    mess->player->askedForJoiningBy = nullptr;
    mess->player->joiningPlayer = nullptr;

    if (asking->hasSocket()) {
        Message *ans = new Message(H_S_NACK_REAS, (*Serializer::instance()->headToFormatMap)[H_S_NACK_REAS]);
        ans->addData(new std::string("Oponent declined"));
        ans->player = asking;
        Sender::instance()->registerMessage(ans);
    }

    asking->setAvailible(true);
    asked->setAvailible(true);
}

void processGameMove(Message *message) {
    Player *turnPlayer = message->player;
    Game *game = GameManager::instance()->getGameByPlayer(turnPlayer);

    Player *oponent;
    if (game->getPlayer1()->getName() == turnPlayer->getName()) {
        oponent = game->getPlayer2();
    } else {
        oponent = game->getPlayer1();
    }

    /* Turn processing !!! */
    uint32_t cardType = *((uint32_t *) message->getPayload()->at(0));
    uint32_t moveI    = *((uint32_t *) message->getPayload()->at(1));
    uint32_t moveJ    = *((uint32_t *) message->getPayload()->at(2));

    game->field[moveI][moveJ] = cardType;

    /* Was it winning move? */
    if (GameManager::instance()->isGameWon(game, moveI, moveJ)) {
        /* Inform both */
        Message *turnInfo1 = new Message(H_S_GAME_WON, (*Serializer::instance()->headToFormatMap)[H_S_GAME_WON]);
        turnInfo1->addData(new std::string(turnPlayer->getName()));
        turnInfo1->addData(message->getPayload()->at(0));
        turnInfo1->addData(message->getPayload()->at(1));
        turnInfo1->addData(message->getPayload()->at(2));

        Message *turnInfo2 = new Message(H_S_GAME_WON, (*Serializer::instance()->headToFormatMap)[H_S_GAME_WON]);
        turnInfo2->addData(new std::string(turnPlayer->getName()));
        turnInfo2->addData(message->getPayload()->at(0));
        turnInfo2->addData(message->getPayload()->at(1));
        turnInfo2->addData(message->getPayload()->at(2));

        turnInfo1->player = turnPlayer;
        turnInfo2->player = oponent;

        Sender::instance()->registerMessage(turnInfo1);
        Sender::instance()->registerMessage(turnInfo2);

        /* Delete game record */
        turnPlayer->setInGame(false);
        oponent->setInGame(false);
        turnPlayer->setAvailible(true);
        oponent->setAvailible(true);


        GameManager::instance()->deleteGame(game);

        return;
    }

    game->playerOnTurn = oponent;
    /* ******************* */

    Message *turnInfo = new Message(H_S_GAME_MOVE, (*Serializer::instance()->headToFormatMap)[H_S_GAME_MOVE]);
    turnInfo->addData(message->getPayload()->at(0));
    turnInfo->addData(message->getPayload()->at(1));
    turnInfo->addData(message->getPayload()->at(2));
    turnInfo->player = oponent;
    Sender::instance()->registerMessage(turnInfo);
}

void proceedGameEnding(Message *message) {
    Player *p = message->player;
    Game *game = GameManager::instance()->getGameByPlayer(p);

    Player *oponent;
    if (game->getPlayer1()->getName() == p->getName()) {
        oponent = game->getPlayer2();
    } else {
        oponent = game->getPlayer1();
    }


    if (oponent->lost) {
        Server::instance()->deletePlayer(oponent);
        p->setAvailible(true);
        p->setInGame(false);
    } else {
        p->setInGame(false);
        oponent->setInGame(false);
        p->setAvailible(true);
        oponent->setAvailible(true);

        Message *endInfo = new Message(H_S_GAME_ENDED, (*Serializer::instance()->headToFormatMap)[H_S_GAME_ENDED]);
        endInfo->player = oponent;
        Sender::instance()->registerMessage(endInfo);
    }

    GameManager::instance()->deleteGame(game);
}

void proceedPlayerLogout(Message *message) {
    Player *p = message->player;

    if (p->isInGame()) {
        proceedGameEnding(message);
    } else {
        Server::instance()->proceedPlayerDisconnection(p->getSocket());
    }
}

void proceedPlayerLost(Message *message) {
    Player *lost = message->player;

    Game *game = GameManager::instance()->getGameByPlayer(lost);
    Player *oponent;
    if (game->getPlayer1()->getName() == lost->getName()) {
        oponent = game->getPlayer2();
    } else {
        oponent = game->getPlayer1();
    }

    lost->lost = true;

    /* If oponent not lost, inform him. Otherwise delete game and players */
    if (oponent->lost) {
        GameManager::instance()->deleteGame(game);
        oponent->setInGame(false);
        lost->setInGame(false);

        Server::instance()->deletePlayer(oponent);
        Server::instance()->deletePlayer(lost);
    } else {
        Message *info = new Message(H_S_PL_LOSS, (*Serializer::instance()->headToFormatMap)[H_S_PL_LOSS]);
        info->player = oponent;
        Sender::instance()->registerMessage(info);
    }
}

void CommandManager::run() {
    Message *actualCommand;

    while (true) {
        while (!this->commandQueue->empty()) {
            actualCommand = this->commandQueue->pop();

            uint32_t header = actualCommand->getHeader();
            switch (header) {
                case H_C_NEW_PL: {
                    proceedNewPlayerConnection(actualCommand);
                    break;
                }
                case H_C_PL_RECONN: {
                    proceedPlayerReconnection(actualCommand);
                    break;
                }
                case H_C_PL_LIST_REQ: {
                    proceedPlayerListRequest(actualCommand);
                    break;
                }
                case H_C_PL_LOGOUT: {
                    proceedPlayerLogout(actualCommand);
                    break;
                }
                case H_C_JOIN_PLAYER: {
                    proceedPlayerJoining(actualCommand);
                    break;
                }
                case H_C_DECLIN_JOIN: {
                    proceedDeclineJoin(actualCommand);
                    break;
                }
                case H_C_ACCEPT_JOIN: {
                    proceedAcceptJoin(actualCommand);
                    break;
                }
                case H_C_GAME_MOVE: {
                    processGameMove(actualCommand);
                    break;
                }
                case INTERNAL_PL_LOST: {
                    proceedPlayerLost(actualCommand);
                }
            }

            delete actualCommand;
        }
        usleep(100000);
    }
}

void CommandManager::registerCommand(Message *message) {
    //LOG_DEBUG("Registered command");
    this->commandQueue->push(message);
}
