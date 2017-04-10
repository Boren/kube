#pragma once

#include <string>

//! Contains functions for outputting debug information and warnings
class Log {
public:
    //! \enum Logging Levels
    enum Level {
        FATAL = 0, //!< Severe errors that will probably lead to a crash.
        ERROR = 1, //!< Errors that might allow the engine to continue running.
        WARN = 2, //!< Warns about potentially dangerous situations.
        DEBUG = 3 //!< Information events that are useful to debug.
    };

    //! Prints a log message to console in format: "[Time - LEVEL - Caller] Message"
    /*!
      \param level Level of message
      \param caller Name of class printing the message
      \param message Message to print
    */
    static void message(Level level, std::string caller, std::string message);

    //! Set the minimum level to output
    /*!
      \param level Minimum level
    */
    static void setLevel(Level level);

private:
    static Level currentLevel;
};
