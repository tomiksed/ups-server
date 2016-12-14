//
// Created by justme on 14.12.16.
//

#include "Message.h"

Message::Message(uint32_t header, MessageType msgType) {
    this->header = header;
    this->msgType = msgType;

    this->payload = new std::vector<void *>();
}

MessageType Message::getMsgType() const {
    return msgType;
}

const std::vector<void *> *Message::getPayload() const {
    return payload;
}

void Message::addData(void *data) {
    this->payload->push_back(data);
}

uint32_t Message::getHeader() const {
    return header;
}
