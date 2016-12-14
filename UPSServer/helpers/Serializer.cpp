//
// Created by justme on 14.12.16.
//

#include "Serializer.h"

Serializer *Serializer::INSTANCE = new Serializer();

Serializer::Serializer() {}

Serializer *Serializer::instance() {
    return Serializer::INSTANCE;
}

uint8_t *Serializer::serialize(Message *msg, int *resultSize) {
    uint8_t *result;

    if (msg->getMsgType() == MSG_NO_DATA) {
        (*resultSize) = MSG_MIN_SIZE;
        result = this->prepareEmptyPacket(msg->getHeader(), 0);
    } else if (msg->getMsgType() == MSG_STRING) {

    } else if (msg->getMsgType() == MSG_INT) {

    } else if (msg->getMsgType() == MSG_2_INT) {

    } else if (msg->getMsgType() == MSG_3_INT) {

    } else if (msg->getMsgType() == MSG_4_INT) {

    } else if (msg->getMsgType() == MSG_INT_STRING) {

    }

    return result;
}

uint8_t * Serializer::prepareEmptyPacket(uint32_t header, int dataSize) {
    uint8_t *result = new uint8_t[MSG_HEAD_SIZE + MSG_SIZE_SIZE + dataSize + MSG_CHECKSUM_SIZE];

    for (int i = 0; i < MSG_MIN_SIZE + dataSize; i++) {
        result[i] = 0;
    }

    this->copy32intToPlace(result, header, 0);

    return result;
}

void Serializer::copy32intToPlace(uint8_t *target, uint32_t data, int startIndex) {
    target[startIndex + 0] = ((uint8_t *) &data)[3];
    target[startIndex + 1] = ((uint8_t *) &data)[2];
    target[startIndex + 2] = ((uint8_t *) &data)[1];
    target[startIndex + 3] = ((uint8_t *) &data)[0];
}

Message *Serializer::deserialize(uint8_t *data) {
    return nullptr;
}
