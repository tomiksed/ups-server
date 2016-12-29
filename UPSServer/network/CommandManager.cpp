#include "CommandManager.h"
#include "packet.h"
#include "Sender.h"

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
        if (!p->isInGame() && p->hasSocket() && p->hasName() && p->getName() != mess->player->getName()) {
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
    LOG_DEBUG(std::string("Player wants to reconnect: ") + *((std::string *) mess->getPayload()->at(0)));

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

    if (asking->joiningPlayer != nullptr) { /* They can start the game */

        LOG_DEBUG("They are starting the game!");

        /* Put them in a game */

        /* Send them a game start message! */
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
}

void CommandManager::run() {
    Message *actualCommand;

    while (true) {
        if (!this->commandQueue->empty()) {
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
                    /* ooo - dodelat logout */
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
