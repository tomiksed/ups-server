//
// Created by justme on 14.12.16.
//

#ifndef UPSSERVER_SERIALIZER_H
#define UPSSERVER_SERIALIZER_H

#include <map>
#include "../datatypes/Message.h"
#include "Logger.h"

class Serializer {
public:

    static Serializer *instance();

    uint8_t *serialize(Message *msg, int *resultSize);

    Message *deserialize(uint8_t *data);

private:

    static Serializer *INSTANCE;

    Serializer();

    uint8_t *prepareEmptyPacket(uint32_t header, int dataSize);

    void copy32intToPlace(uint8_t *target, uint32_t data, int startIndex);
};


#endif //UPSSERVER_SERIALIZER_H
