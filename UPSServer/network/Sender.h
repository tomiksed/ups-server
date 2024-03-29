//
// Created by justme on 13.12.16.
//

#ifndef UPSSERVER_SENDER_H
#define UPSSERVER_SENDER_H

#include <thread>
#include <sys/socket.h>

#include "../datatypes/ConcurrentQueue.cpp"
#include "packet.h"
#include "../datatypes/Message.h"
#include "../helpers/Serializer.h"

class Sender {
public:

    static Sender *instance();

    void init();
    void start();

    void registerPacket(packet_t *p);
    void registerMessage(Message *mess);

private:
    static Sender *INSTANCE;

    Sender();

    ConcurrentQueue<packet_t *> *pQueue;
    std::thread *sThread;

    void run();
};


#endif //UPSSERVER_SENDER_H
