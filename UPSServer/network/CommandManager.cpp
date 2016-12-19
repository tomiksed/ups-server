#include "CommandManager.h"

CommandManager *CommandManager::INSTANCE = new CommandManager();

CommandManager::CommandManager(){}

CommandManager *CommandManager::instance(){
    return INSTANCE;
}

std::thread *CommandManager::start() {
    this->cmThread = new std::thread(&CommandManager::run, this);
}

void CommandManager::run() {

}
