#include "Server.h"
#include "packet.h"
#include "PacketManager.h"

Server *Server::INSTANCE = new Server();

std::vector<Player *> *Server::players = new std::vector<Player *>();

Server::Server(){}

Server *Server::instance(){
    return INSTANCE;
}

void Server::init(uint16_t listeningPort) {
    int returnValue;

    this->listeningPort = listeningPort;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&myAddr, 0, sizeof(struct sockaddr_in));

    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(listeningPort);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    int enable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        LOG_ERROR("!!! Setsockopt() SO_REUSEADDR !!!");

    returnValue = bind(serverSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in));

    if (returnValue != 0) {
        LOG(log::ERROR, std::string("Couldn't bind socket!"));
        _exit(-1);
    }

    listen(serverSocket, queueSize);
}

std::thread *Server::start() {
    LOG(log::DEBUG, std::string("Server started"));
    this->sThread = new std::thread(&Server::run, this);
    return this->sThread;
}

void Server::proceedPlayerDisconnection(int socket) {
    Player *pl;
    int index = -1;

    for (Player *p : *Server::players) {
        index++;
        if (p->hasSocket() && p->getSocket() == socket) {
            pl = p;
            break;
        }
    }

    if (!pl->isInGame()) {
        LOG_INFO("ERASING PLAYER " + pl->getName());
        pl->removeSocket();
        pl->joiningPlayer = nullptr;
        this->players->erase(this->players->begin() + index);
    } else {
        pl->removeSocket();
    }

    close(socket);
    FD_CLR(socket, &clientSocks);
    LOG(log::INFO, std::string("Client disconnected (") + std::to_string(socket) + ")");
}

void Server::run() {
    int fd, returnValue, readSize;
    uint8_t *msgBuffer;

    FD_ZERO(&clientSocks);
    FD_SET(this->serverSocket, &clientSocks);


    while(true) {
        readSocks = clientSocks;

        returnValue = select(FD_SETSIZE, &readSocks, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (returnValue < 0) {
            LOG(log::ERROR, std::string("Select() error!"));
            _exit(-1);
        }

        for (fd = 3; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &readSocks)) {
                /* New connection - accept it */
                if (fd == this->serverSocket) {
                    /* ooo - tady by se mel parchant pridat do nakyho seznamu hracu */
                    int clientSocket = this->acceptNewConnection();
                    LOG(log::INFO, std::string("New connection (") + std::to_string(clientSocket) + ")");
                    FD_SET(clientSocket, &clientSocks);

                    /* ooo - tutu sracku dodelat */
                    Player *p = new Player(clientSocket);
                    Server::players->push_back(p);

                    /* Message from client or clients disconnection */
                } else {
                    /* How many bytes are waiting to be read */
                    ioctl(fd, FIONREAD, &readSize);

                    /* We've got data for reading */
                    if (readSize > 0) {
                        msgBuffer = new uint8_t[readSize];
                        memset(msgBuffer, 0, readSize);

                        recv(fd, msgBuffer, readSize, 0);

                        this->processData(msgBuffer, readSize, fd);

                    /* Client disconnected */
                    } else {
                        proceedPlayerDisconnection(fd);
                    }
                }
            }
        }
    }
}

void Server::processData(uint8_t *data, int size, int socket) {
    packet_t *packet = new packet_t;

    packet->data = data;

    packet->size = size;

    packet->socket = socket;

    PacketManager::instance()->registerPacket(packet);
}

int Server::acceptNewConnection() {
    int clientSocket = accept(this->serverSocket, (struct sockaddr *) &peerAddr, &lenAddr);

    int optval = 1;
    int optlen = sizeof(optval);


    if(setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        LOG(log::ERROR, std::string("Setsockopt error!"));
        close(clientSocket);
        _exit(-1);
    }


    optval = 3;

    /* tcp_keepalive_time */
    if(setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPIDLE, &optval, optlen) < 0) {
        LOG(log::ERROR, std::string("Setsockopt error!"));
        close(clientSocket);
        _exit(-1);
    }

    optval = 2;

    /* tcp_keepalive_probes */
    if(setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPCNT, &optval, optlen) < 0) {
        LOG(log::ERROR, std::string("Setsockopt error!"));
        close(clientSocket);
        _exit(-1);
    }

    optval = 2;

    /* tcp_keepalive_intvl */
    if(setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) {
        LOG(log::ERROR, std::string("Setsockopt error!"));
        close(clientSocket);
        _exit(-1);
    }

    return clientSocket;
}

Player *Server::getPlayerBySocket(int socket) {
    for (Player *p : *Server::players) {
        if (p->hasSocket() && p->getSocket() == socket) {
            return p;
        }
    }

    return nullptr;
}

Player *Server::getPlayerByName(std::string name) {
    for (Player *p : *Server::players) {
        if (p->getName() == name) {
            return p;
        }
    }

    return nullptr;
}

