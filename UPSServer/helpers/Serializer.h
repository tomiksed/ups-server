//
// Created by justme on 14.12.16.
//

#ifndef UPSSERVER_SERIALIZER_H
#define UPSSERVER_SERIALIZER_H

#include <map>
#include <string>
#include "../datatypes/Message.h"
#include "Logger.h"

void fillMapWithValues(std::map<uint32_t, std::string> *headToFormatMap);

class Serializer {
public:

    static Serializer *instance();

    uint8_t *serialize(Message *msg, int *resultSize);

    Message *deserialize(uint8_t *data);

    std::map<uint32_t, std::string> *headToFormatMap;

private:

    static Serializer *INSTANCE;

    Serializer();

    /* Message serialization */
    uint8_t *prepareEmptyPacket(uint32_t header, int dataSize);

    void copy32intToPlace(uint8_t *target, uint32_t data, int startIndex);

    void countAndPlaceChecksum(uint8_t *target, int size);

    int getMessageSizeByPayload(Message *msg);

    void putDataToMessage(uint8_t *target, std::string format, const std::vector<void *> *payload);



    /* Message deserialization */
    uint32_t parse32intFromArray(uint8_t *source, int index);

    std::string *getStringFromMessage(uint8_t *source, int size, int index);
};


#endif //UPSSERVER_SERIALIZER_H
