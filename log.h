#ifndef DIALOG_LOG_H_
#define DIALOG_LOG_H_

//#include <alog/Logger.h>
//#include <alog/Configurator.h>
#include "../mylib/Logger.h"
#include "../mylib/Configurator.h"
#include <iostream>

#define ROOT_LOG_CONFIG() alog::Configurator::configureRootLogger()
#define LOG_CONFIG(filename) do {                           \
    try {                                                   \
      alog::Configurator::configureLogger(filename);        \
    } catch (std::exception &e) {                           \
      std::cerr << "Error!!! Fail to configure logger"      \
                << e.what() << std::endl;                   \
      exit(-1);                                             \
    }                                                       \
} while (false)

#define ROOT_LOG_SETLEVEL(level) \
    alog::Logger::getRootLogger()->setLevel(alog::LOG_LEVEL_##level)
#define DIALOG_SEG_LOG_DECLARE() static alog::Logger *_logger
#define DIALOG_SEG_LOG_SETUP(n,c) alog::Logger *c::_logger \
    = alog::Logger::getLogger("dialog." #n "."  #c)

#define LOG_SETUP_TEMPLATE(n,c, T) template <typename T>        \
    alog::Logger *c<T>::_logger                                 \
    = alog::Logger::getLogger("dialog." #n "."  #c)

#define LOG(level, format, args) \
        ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args)

#define DECLARE_AND_SETUP_LOGGER(n) static alog::Logger *_logger \
    = alog::Logger::getLogger("dialog." #n)
#define LOG_SHUTDOWN() alog::Logger::shutdown()
#define LOG_FLUSH() alog::Logger::flushAll()

#endif  // DIALOG_LOG_H_
