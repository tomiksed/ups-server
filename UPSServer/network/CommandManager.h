//
// Created by justme on 7.12.16.
//

#ifndef UPSSERVER_COMMANDMANAGER_H
#define UPSSERVER_COMMANDMANAGER_H


class CommandManager {

public:

    static CommandManager *instance();

private:
    static CommandManager *INSTANCE;

    CommandManager();
};


#endif //UPSSERVER_COMMANDMANAGER_H
