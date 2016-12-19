//
// Created by justme on 14.12.16.
//

#include "Message.h"

Message::Message(uint32_t header, std::string msgFormat) {
    this->header = header;
    this->msgFormat = msgFormat;

    this->payload = new std::vector<void *>();
}

std::string Message::getMsgFormat() {
    return msgFormat;
}

const std::vector<void *> *Message::getPayload() {
    return payload;
}

void Message::addData(void *data) {
    if (payload->size() == msgFormat.length()) {
        LOG_ERROR("!!! PAYLOAD IS AT MAX SIZE !!!");
    }
    this->payload->push_back(data);
}

uint32_t Message::getHeader() {
    return header;
}
