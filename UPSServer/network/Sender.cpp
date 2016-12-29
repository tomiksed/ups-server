//
// Created by justme on 13.12.16.
//

#include "Sender.h"
#include "packet.h"

Sender *Sender::INSTANCE = new Sender();

Sender *Sender::instance() {
    return Sender::INSTANCE;
}

Sender::Sender() {}

void Sender::init() {
    this->pQueue = new ConcurrentQueue<packet_t *>();
}

void Sender::start() {
    this->sThread = new std::thread(&Sender::run, this);
}
#include <unistd.h>
void Sender::run() {
    packet_t *packet;

    while (true) {
        if (!this->pQueue->empty()) {
            packet = this->pQueue->pop();

            send(packet->socket, packet->data, packet->size, MSG_NOSIGNAL);

            delete packet->data;
            delete packet;
        }
        usleep(100000);
    }
}

void Sender::registerPacket(packet_t *p) {
    this->pQueue->push(p);
}

void Sender::registerMessage(Message *mess) {
    packet_t *p = new packet_t;

    p->data = Serializer::instance()->serialize(mess, &p->size);
    p->socket = mess->player->getSocket();

    this->registerPacket(p);
}
