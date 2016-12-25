//
// Created by justme on 10.12.16.
//

#ifndef UPSSERVER_SERVER_H
#define UPSSERVER_SERVER_H

#include <cstdint>

#include <iostream>
#include <thread>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>

#include "../globals.h"
#include "packet.h"
#include "../helpers/Logger.h"
#include "../game/Player.h"

/*
 * Server - singleton class responsible for accepting connections,
 * reading from peers and closing connections
 */
class Server {

public:

    static Server *instance();

    /* Creates socket */
    void init(uint16_t listeningPort);
    /* Starts listening routine */
    std::thread *start();

    /* ooo */
    static std::vector<Player *> *players;
    Player *getPlayerBySocket(int socket);

private:
    static Server *INSTANCE;

    /* Port which the server is listening on */
    uint16_t listeningPort;

    /* Size of the pool for waiting peers before being accepted */
    int queueSize = 10;

    /* Stores the descriptor of the server socket */
    int serverSocket = 0;
    socklen_t lenAddr;
    struct sockaddr_in myAddr, peerAddr;

    /* Thread which the server runs in */
    std::thread *sThread;

    Server();

    /* Main method for the thread */
    void run();

    /* Accepts new connections and sets socket flags */
    int acceptNewConnection();

    /* Processes accepted data - creates packet and hands it to the PacketManager */
    void processData(uint8_t *data, int size, int socket);
};


#endif //UPSSERVER_SERVER_H
