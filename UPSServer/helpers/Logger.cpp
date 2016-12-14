//
// Created by justme on 11.12.16.
//

#include <iostream>
#include <sys/time.h>

#include "Logger.h"

Logger *Logger::INSTANCE = new Logger();

Logger *Logger::instance() {
    return INSTANCE;
}

void Logger::init(log::LogLevel logLevel) {
    this->logLevel = logLevel;
}

void Logger::log(log::LogLevel level, std::string msg) {
    this->lMutex.lock();

        if (level >= this->logLevel) {
            timeval curTime;
            gettimeofday(&curTime, NULL);
            int milli = curTime.tv_usec / 1000;

            char buffer[80];
            strftime(buffer, 80, "%H:%M:%S", localtime(&curTime.tv_sec));
            char currentTime[84] = "";
            sprintf(currentTime, "[%s:%03d]: ", buffer, milli);

            if (level == log::ERROR) {
                std::cerr << "ERROR " << currentTime << msg << std::endl;
            } else {
                std::string severityString("");
                switch (level) {
                    case log::WARNING: severityString = "WARNING "; break;
                    case log::INFO: severityString = "INFO "; break;
                    case log::DEBUG: severityString = "DEBUG "; break;
                }
                std::cout << severityString << currentTime << msg << std::endl;
            }
        }

    this->lMutex.unlock();
}

Logger::Logger() {

}
