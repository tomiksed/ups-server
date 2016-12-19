#include <iostream>
#include "network/PacketManager.h"
#include "network/Server.h"
#include "network/Sender.h"
#include "datatypes/Message.h"
#include "helpers/Serializer.h"


#include <cstdint>

int main(int argc, char *argv[]) {
    /*Logger::instance()->init(log::DEBUG);

    Sender::instance()->init();
    Sender::instance()->start();

    PacketManager::instance()->init();
    PacketManager::instance()->start();

    Server::instance()->init(55555);
    std::thread *serverThread = Server::instance()->start();

     /**/

    /*uint32_t pokus = (0x01 << 24) | (0x02 << 16) | (0x03 << 8) | (0x04);
    std::cout << "Pokus is: " << pokus << std::endl;
    if (((uint8_t *) &pokus)[0] == 0x01) {
        std::cout << "Big endian!" << std::endl;
    } else {
        std::cout << "Little endian!" << std::endl;
    }

    std::cout << "First byte =" << std::to_string(((uint8_t *) &pokus)[0]) << std::endl; /**/

    /*uint32_t pokus = 5;
    Message *pokusna = new Message(POKUSTON, std::string("siissis"));
    pokusna->addData((void *) new std::string("Toto je pokuston pokusovaty"));
    pokusna->addData(&pokus);
    uint32_t pokusa = 0xa01536;
    pokusna->addData(&pokusa);
    pokusna->addData((void *) new std::string("didi;tomik;luisa;pepa;franta"));
    pokusna->addData((void *) new std::string("Ahoj vole, jak se mas?"));
    pokusna->addData(&pokus);
    pokusna->addData((void *) new std::string("Dobre"));

    int resultSize = 0;
    uint8_t *serZprava = Serializer::instance()->serialize(pokusna, &resultSize);

    Message *deserializovana = Serializer::instance()->deserialize(serZprava);

    uint32_t *intPayload;
    std::string *stringPayload;
    const std::vector<void *> *payload = deserializovana->getPayload();

    int i = 0;
    for (char f : deserializovana->getMsgFormat()) {
        switch (f) {
            case 'i': {
                intPayload = (uint32_t *) payload->at(i);

                LOG_DEBUG("INT payload: " + std::to_string((*intPayload)));

                break;
            }
            case 's': {
                stringPayload = (std::string *) payload->at(i);

                LOG_DEBUG("STRING payload: " + *stringPayload);

                break;
            }
        }
        i++;
    }/**/

    /*for (int i = 0; i < resultSize; i++) {
        LOG_DEBUG(std::string("") + std::to_string(serZprava[i]));
    }
    /**/
    //serverThread->join();

    return 0;
}