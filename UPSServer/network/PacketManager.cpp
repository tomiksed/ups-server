#include "PacketManager.h"
#include "packet.h"
#include "Sender.h"
#include "../datatypes/Message.h"
#include "../helpers/Serializer.h"
#include "Server.h"
#include "CommandManager.h"

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

void PacketManager::run() {
    packet_t *packet;

    while(true) {
        if (!this->packetQueue->empty()) {
            packet = this->packetQueue->pop();

            Message *mess = Serializer::instance()->deserialize(packet->data);

            if (mess != nullptr) {
                mess->player = Server::instance()->getPlayerBySocket(packet->socket);

                CommandManager::instance()->registerCommand(mess);
            }

            delete packet->data;
            delete packet;
        }
    }
}

void PacketManager::registerPacket(packet_t *packet) {
    this->packetQueue->push(packet);
}
