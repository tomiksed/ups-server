#include "CommandManager.h"

CommandManager *CommandManager::INSTANCE = new CommandManager();

CommandManager::CommandManager(){}

CommandManager *CommandManager::instance(){
    return INSTANCE;
}