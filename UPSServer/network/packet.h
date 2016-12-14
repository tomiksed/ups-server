//
// Created by justme on 7.12.16.
//

#ifndef UPSSERVER_PACKET_H
#define UPSSERVER_PACKET_H

#include <cstdint>

typedef struct {
    int socket;
    int size;
    uint8_t *data;
} packet_t;

#endif //UPSSERVER_PACKET_H
