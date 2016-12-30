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
#define MSG_STRING_LEN_SIZE 4

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

#define H_S_NACK_REAS     0x00010008


#define H_C_JOIN_PLAYER   0x00000009

#define H_S_JOIN_PLAYER   0x0001000A

#define H_C_DECLIN_JOIN   0x0000000B
#define H_C_ACCEPT_JOIN   0x0000000C

#define H_S_GAME_START    0x0001000D

#define H_C_GAME_MOVE     0x0000000E
#define H_S_GAME_MOVE     0x0001000F


#include <cstdint>
#include <vector>
#include <string>

#include "../helpers/Logger.h"
#include "../game/Player.h"


class Message {
public:

    Message(uint32_t header, std::string format);

    void addData(void *data);



    std::string getMsgFormat();

    const std::vector<void *> *getPayload();

    uint32_t getHeader();

    Player *player;

private:
    uint32_t header;

    std::string msgFormat;

    std::vector<void *> *payload;

};


#endif //UPSSERVER_MESSAGE_H
