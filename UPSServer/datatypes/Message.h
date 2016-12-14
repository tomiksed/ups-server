//
// Created by justme on 14.12.16.
//

#ifndef UPSSERVER_MESSAGE_H
#define UPSSERVER_MESSAGE_H

/* Header + size + checksum */
#define MSG_MIN_SIZE 9
#define MSG_HEAD_SIZE 4
#define MSG_SIZE_SIZE 4
#define MSG_CHECKSUM_SIZE 1

#define H_C_NACK          0x00000000
#define H_C_ACK           0x00000001
#define H_C_PING          0x00000002

#define H_S_NACK          0x00010000
#define H_S_ACK           0x00010001
#define H_S_PING          0x00010002

#define H_C_NEW_PL        0x00000003
#define H_C_PL_RECONN     0x00000004
#define H_C_PL_LOGOUT     0x00000005

#define H_C_PL_LIST_REQ   0x00000006
#define H_S_PL_LIST_ANS   0x00010007


#include <cstdint>
#include <vector>

/* ooo - reflektovat i do Serializeru!! */
enum MessageType {
    MSG_NO_DATA,
    MSG_STRING,
    MSG_INT,
    MSG_2_INT,
    MSG_3_INT,
    MSG_4_INT,
    MSG_INT_STRING
};

class Message {
public:

    Message(uint32_t header, MessageType msgType);

    void addData(void *data);



    MessageType getMsgType() const;

    const std::vector<void *> *getPayload() const;

    uint32_t getHeader() const;

private:
    uint32_t header;

    MessageType msgType;

    std::vector<void *> *payload;
};


#endif //UPSSERVER_MESSAGE_H
