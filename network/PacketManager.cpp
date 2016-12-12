#include "PacketManager.h"
#include "packet.h"

PacketManager *PacketManager::INSTANCE = new PacketManager();

void PacketManager::init() {
    this->packetQueue = new ConcurrentQueue<packet_t *>();
}

PacketManager::PacketManager(){}

PacketManager *PacketManager::instance(){
    return INSTANCE;
}

void PacketManager::start() {
    LOG(log::DEBUG, std::string("Packet manager started"));
    this->pmThread = new std::thread(&PacketManager::run, this);
}

#include <unistd.h>
void PacketManager::run() {
    packet_t *packet;

    while(true) {
        if (!this->packetQueue->empty()) {
            packet = this->packetQueue->pop();

            //LOG(log::DEBUG, std::string("Packet: ") + (char *) packet->data);
        }

        sleep(1);
    }
}

void PacketManager::registerPacket(packet_t *packet) {
    this->packetQueue->push(packet);
}
