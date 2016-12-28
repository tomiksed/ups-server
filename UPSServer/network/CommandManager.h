//
// Created by justme on 7.12.16.
//

#ifndef UPSSERVER_COMMANDMANAGER_H
#define UPSSERVER_COMMANDMANAGER_H

#include <thread>

#include "../helpers/Logger.h"
#include "../datatypes/ConcurrentQueue.cpp"
#include "../datatypes/Message.h"
#include "../helpers/Serializer.h"
#include "Server.h"

class CommandManager {

public:

    static CommandManager *instance();

    std::thread *start();

    void init();

    void registerCommand(Message *message);

private:
    static CommandManager *INSTANCE;

    CommandManager();

    std::thread *cmThread;

    ConcurrentQueue<Message *> *commandQueue;

    void run();
};


#endif //UPSSERVER_COMMANDMANAGER_H
