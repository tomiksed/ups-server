#include <iostream>
#include "network/PacketManager.h"
#include "network/Server.h"
#include "helpers/Logger.h"

int main(int argc, char *argv[]) {
    Logger::instance()->init(log::DEBUG);


    PacketManager::instance()->init();
    PacketManager::instance()->start();

    Server::instance()->init(55555);
    std::thread *serverThread = Server::instance()->start();

    serverThread->join();

    return 0;
}