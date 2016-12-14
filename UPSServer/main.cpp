#include <iostream>
#include "network/PacketManager.h"
#include "network/Server.h"
#include "network/Sender.h"

#include <cstdint>

int main(int argc, char *argv[]) {
    Logger::instance()->init(log::DEBUG);

    Sender::instance()->init();
    Sender::instance()->start();

    PacketManager::instance()->init();
    PacketManager::instance()->start();

    Server::instance()->init(55555);
    std::thread *serverThread = Server::instance()->start();

    serverThread->join(); /**/

    /*uint32_t pokus = (0x01 << 24) | (0x02 << 16) | (0x03 << 8) | (0x04);
    std::cout << "Pokus is: " << pokus << std::endl;
    if (((uint8_t *) &pokus)[0] == 0x01) {
        std::cout << "Big endian!" << std::endl;
    } else {
        std::cout << "Little endian!" << std::endl;
    }

    std::cout << "First byte =" << std::to_string(((uint8_t *) &pokus)[0]) << std::endl; /**/

    return 0;
}