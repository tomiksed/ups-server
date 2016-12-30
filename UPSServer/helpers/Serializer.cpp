//
// Created by justme on 14.12.16.
//

#include "Serializer.h"

Serializer *Serializer::INSTANCE = new Serializer();

Serializer::Serializer() {
    this->headToFormatMap = new std::map<uint32_t, std::string>();

    fillMapWithValues(this->headToFormatMap);
}

Serializer *Serializer::instance() {
    return Serializer::INSTANCE;
}

uint8_t *Serializer::serialize(Message *msg, int *resultSize) {
    uint8_t *result;
    int messageSize, payloadSize;

    if (msg->getMsgFormat() == "") {
        messageSize = MSG_MIN_SIZE;
        result = this->prepareEmptyPacket(msg->getHeader(), 0);
        this->countAndPlaceChecksum(result, messageSize);
    } else {
        if (msg->getPayload()->size() != msg->getMsgFormat().length()) {
            LOG_ERROR("!!! Message payload size doesn't equal the format string !!!");
        }

        payloadSize = getMessageSizeByPayload(msg);
        messageSize = MSG_MIN_SIZE + payloadSize;
        result = this->prepareEmptyPacket(msg->getHeader(), payloadSize);

        this->putDataToMessage(result, msg->getMsgFormat(), msg->getPayload());

        this->countAndPlaceChecksum(result, messageSize);
    }


    (*resultSize) = messageSize;
    return result;
}

void Serializer::putDataToMessage(uint8_t *target, std::string format, const std::vector<void *> *payload) {
    int index = MSG_HEAD_SIZE + MSG_SIZE_SIZE;

    std::string *stringPayload;
    uint32_t *intPayload;

    int i = 0;
    for (char f : format) {
        switch (f) {
            case 'i': {
                intPayload = (uint32_t *) payload->at(i);
                this->copy32intToPlace(target, *intPayload, index);

                index += 4;
                break;
            }
            case 's': {
                stringPayload = (std::string *) payload->at(i);
                uint32_t stringSize = stringPayload->size();

                this->copy32intToPlace(target, stringSize, index);
                index += 4;

                for (char sf : *stringPayload) {
                    target[index] = sf;
                    index++;
                }
                break;
            }
        }
        i++;
    }
}

int Serializer::getMessageSizeByPayload(Message *msg) {
    const std::vector<void *> *payload = msg->getPayload();
    std::string format = msg->getMsgFormat();

    int resultSize = 0;
    int i = 0;

    for (char f : format) {
        switch (f) {
            case 'i': {
                resultSize += 4;

                break;
            }
            case 's': {
                resultSize += MSG_STRING_LEN_SIZE;
                resultSize += ((std::string *) payload->at(i))->length();

                break;
            }
        }
        i++;
    }

    return resultSize;
}

uint8_t * Serializer::prepareEmptyPacket(uint32_t header, int dataSize) {
    int messageSize = MSG_HEAD_SIZE + MSG_SIZE_SIZE + dataSize + MSG_CHECKSUM_SIZE;
    uint8_t *result = new uint8_t[messageSize];

    for (int i = 0; i < MSG_MIN_SIZE + dataSize; i++) {
        result[i] = 0;
    }

    this->copy32intToPlace(result, header, 0);
    this->copy32intToPlace(result, messageSize, 4);

    return result;
}

void Serializer::countAndPlaceChecksum(uint8_t *target, int size) {
    uint8_t checkSum = 0;

    for (int i = 0; i < size - 1; i++) {
        checkSum += target[i];
        checkSum %= 128;
    }

    target[size - 1] = checkSum;
}

void Serializer::copy32intToPlace(uint8_t *target, uint32_t data, int startIndex) {
    target[startIndex + 0] = ((uint8_t *) &data)[3];
    target[startIndex + 1] = ((uint8_t *) &data)[2];
    target[startIndex + 2] = ((uint8_t *) &data)[1];
    target[startIndex + 3] = ((uint8_t *) &data)[0];
}

Message *Serializer::deserialize(uint8_t *data) {
    uint32_t header = parse32intFromArray(data, 0);
    uint32_t size = parse32intFromArray(data, 4);

    int index = 8;

    if (headToFormatMap->find(header) == headToFormatMap->end()) {
        LOG_WARNING("RECEIVED INVALID MESSAGE");
        return nullptr;
    }

    std::string format = (*headToFormatMap)[header];

    Message *message = new Message(header, format);

    for (char c : format) {
        switch (c) {
            case 'i': {
                uint32_t *intPayload = new uint32_t;
                (*intPayload) = parse32intFromArray(data, index);

                message->addData(intPayload);

                index += 4;
                break;
            }
            case 's': {
                uint32_t stringLength = parse32intFromArray(data, index);
                index += 4;

                std::string *stringPayload = getStringFromMessage(data, stringLength, index);

                message->addData(stringPayload);

                index += stringLength;
                break;
            }
        }
    }

    return message;
}

std::string *Serializer::getStringFromMessage(uint8_t *source, int size, int index) {
    std::string *payload = new std::string("");

    for (int i = 0; i < size; i++) {
        (*payload) += (char) source[index + i];
    }

    return payload;
}

uint32_t Serializer::parse32intFromArray(uint8_t *source, int index) {
    uint32_t result = 0;

    result = source[index] << 24 | source[index + 1] << 16 | source[index + 2] << 8 | source[index + 3];

    return result;
}


void fillMapWithValues(std::map<uint32_t, std::string> *headToFormatMap) {
    (*headToFormatMap)[H_C_NACK] = std::string("");
    (*headToFormatMap)[H_C_ACK] = std::string("");
    (*headToFormatMap)[H_C_PING] = std::string("");

    (*headToFormatMap)[H_S_NACK] = std::string("");
    (*headToFormatMap)[H_S_ACK] = std::string("");
    (*headToFormatMap)[H_S_PING] = std::string("");

    (*headToFormatMap)[H_C_NEW_PL] = std::string("s"); // jmeno, pod kterym se chci zaregistrovat
    (*headToFormatMap)[H_C_PL_RECONN] = std::string("s"); // jmeno, pod kterym se chci znovu pripojit
    (*headToFormatMap)[H_C_PL_LOGOUT] = std::string("");

    (*headToFormatMap)[H_C_PL_LIST_REQ] = std::string("");
    (*headToFormatMap)[H_S_PL_LIST_ANS] = std::string("is"); // pocet hracu a jejich hmena oddelena ';'

    (*headToFormatMap)[H_S_NACK_REAS] = std::string("s"); // duvod odmitnuti

    (*headToFormatMap)[H_C_JOIN_PLAYER] = std::string("s"); // jmeno hrace k pripojeni

    (*headToFormatMap)[H_S_JOIN_PLAYER] = std::string("s"); // jmeno hrace, kterej se chce pripojit

    (*headToFormatMap)[H_C_DECLIN_JOIN] = std::string("");
    (*headToFormatMap)[H_C_ACCEPT_JOIN] = std::string("");

    (*headToFormatMap)[H_S_GAME_START] = std::string("ss"); // jmeno hrace, co zacina + jmeno onopenta

    (*headToFormatMap)[H_C_GAME_MOVE] = std::string("iii"); // typ karty, i, j
    (*headToFormatMap)[H_S_GAME_MOVE] = std::string("iii"); // typ karty, i, j

    (*headToFormatMap)[H_S_GAME_WON] = std::string("siii"); // jmeno viteze + vitezny tah
}