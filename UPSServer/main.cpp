#include <cstdlib>
#include <iostream>
#include <string>
#include "network/PacketManager.h"
#include "network/Server.h"
#include "network/Sender.h"
#include "helpers/MainOptions.h"
#include "network/CommandManager.h"


#include <cstdint>
#include <stdlib.h>

void printHelp() {
    std::cout << "Usage: traxServer [options]" << std::endl
              << "\tPossible options:" << std::endl
              << "\t\t-p <port number>     - port to listen on" << std::endl
              << "\t\t-l <log level>     - log level (0 - 3)" << std::endl;
}

bool isNumeric(std::string str) {
    for (char c : str) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

int loglevel = 1;
int port = 55555;

void doParameters(int argc, char *argv[]) {
    MainOptions mo(argc, argv);

    if (mo.hasKey("-h")) {
        printHelp();
        _exit(0);
    }

    if (mo.hasKey("-l")) {
        MainOptions::Option* opt = mo.getParamFromKey("-l");
        std::string type = opt ? (*opt).second : "";

        if (type != "") {
            if (isNumeric(type) && atoi(type.c_str()) <= 3 && atoi(type.c_str()) >= 0) {
                loglevel = atoi(type.c_str());
            } else {
                std::cerr << "Wrong log level option, using default" << std::endl;
            }
        } else {
            printHelp();
            _exit(-1);
        }
    }

    if (mo.hasKey("-p")) {
        MainOptions::Option* opt = mo.getParamFromKey("-p");
        std::string type = opt ? (*opt).second : "";

        if (type != "") {
            if (isNumeric(type) && atoi(type.c_str()) <= 65535 && atoi(type.c_str()) > 0) {
                port = atoi(type.c_str());
            } else {
                std::cerr << "Wrong port option, using default" << std::endl;
            }
        } else {
            printHelp();
            _exit(-1);
        }
    }
}

int main(int argc, char *argv[]) {
    doParameters(argc, argv);

    switch (loglevel) {
        case 0: {
            Logger::instance()->init(log::DEBUG);
            break;
        }
        case 1: {
            Logger::instance()->init(log::INFO);
            break;
        }
        case 2: {
            Logger::instance()->init(log::WARNING);
            break;
        }
        case 3: {
            Logger::instance()->init(log::ERROR);
            break;
        }
    }

    Sender::instance()->init();
    Sender::instance()->start();

    PacketManager::instance()->init();
    PacketManager::instance()->start();

    CommandManager::instance()->init();
    CommandManager::instance()->start();

    Server::instance()->init(port);

    LOG_INFO("Starting server at port " + std::to_string(port));
    std::thread *serverThread = Server::instance()->start();


    serverThread->join();

    return 0;
}