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

void proceedNewPlayerConnection(Message *mess) {
    LOG_DEBUG(std::string("New player wants to connect: ") + *((std::string *) mess->getPayload()->at(0)));
}

void proceedPlayerReconnection(Message *mess) {
    LOG_DEBUG(std::string("Player wants to reconnect: ") + *((std::string *) mess->getPayload()->at(0)));

}

void CommandManager::run() {
    Message *actualCommand;

    while (true) {
        if (!this->commandQueue->empty()) {
            actualCommand = this->commandQueue->pop();

            if (actualCommand->getHeader() == H_C_NEW_PL) {
                proceedNewPlayerConnection(actualCommand);
            } else if (actualCommand->getHeader() == H_C_PL_RECONN) {
                proceedPlayerReconnection(actualCommand);
            }

            delete actualCommand;
        }
    }
}

void CommandManager::registerCommand(Message *message) {
    LOG_DEBUG("Registered command");
    this->commandQueue->push(message);
}
