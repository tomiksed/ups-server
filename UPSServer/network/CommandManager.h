//
// Created by justme on 7.12.16.
//

#ifndef UPSSERVER_COMMANDMANAGER_H
#define UPSSERVER_COMMANDMANAGER_H

#include <thread>

class CommandManager {

public:

    static CommandManager *instance();

    std::thread *start();

private:
    static CommandManager *INSTANCE;

    CommandManager();

    std::thread *cmThread;

    void run();
};


#endif //UPSSERVER_COMMANDMANAGER_H
