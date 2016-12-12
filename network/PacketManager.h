//
// Created by justme on 7.12.16.
//

#ifndef UPSSERVER_PACKETMANAGER_H
#define UPSSERVER_PACKETMANAGER_H

#include <thread>

#include "packet.h"
#include "../datatypes/ConcurrentQueue.cpp"
#include "../helpers/Logger.h"

class PacketManager {

public:

    static PacketManager *instance();

    void init();

    void start();

    void registerPacket(packet_t *packet);

private:
    static PacketManager *INSTANCE;
    PacketManager();

    std::thread *pmThread;
    ConcurrentQueue<packet_t *> *packetQueue;

    void run();
};


#endif //UPSSERVER_PACKETMANAGER_H
