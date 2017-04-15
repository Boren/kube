#include "log.h"
#include <ctime>
#include <iomanip>
#include <iostream>

Log::Level Log::currentLevel = Level::DEBUG;

void Log::message(Log::Level level, std::string caller, std::string message) {
  if (level <= currentLevel) {
    // Get time and format it
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M:%S", timeinfo);
    std::string timeString(buffer);

    // Get loglevel as string
    std::string logLevelString;
    switch (level) {
    case Level::FATAL:
      logLevelString = "FATAL";
      break;
    case Level::ERROR:
      logLevelString = "ERROR";
      break;
    case Level::WARN:
      logLevelString = "WARN";
      break;
    case Level::DEBUG:
      logLevelString = "DEBUG";
      break;
    }

    std::cout << "[" << timeString << " - " << logLevelString << " - " << caller
              << "] " << message << std::endl;
  }
}

void Log::setLevel(Log::Level level) { currentLevel = level; }
