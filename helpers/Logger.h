//
// Created by justme on 11.12.16.
//

#ifndef UPSSERVER_LOGGER_H
#define UPSSERVER_LOGGER_H

#include <mutex>

#define LOG(a, b) Logger::instance()->log((a), (b))

#define LOG_DEBUG(a) Logger::instance()->log(log::DEBUG, (a))
#define LOG_INFO(a) Logger::instance()->log(log::INFO, (a))
#define LOG_WARNING(a) Logger::instance()->log(log::WARNING, (a))
#define LOG_ERROR(a) Logger::instance()->log(log::ERROR, (a))

/* Enum indicating the logging event's severity */
namespace log {
    enum LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3
    };
}

class Logger {
public:

    static Logger *instance();

    void init(log::LogLevel logLevel);

    void log(log::LogLevel level, std::string msg);

private:

    static Logger *INSTANCE;

    log::LogLevel logLevel;

    std::mutex lMutex;

    Logger();
};


#endif //UPSSERVER_LOGGER_H
