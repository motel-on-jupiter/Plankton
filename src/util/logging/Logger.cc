/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "Logger.h"

const char *Logger::kLogPrefixes[] = { "",         //kNoneLevel
    "[TRACE]",  //kTrace
    "[DEBUG]",  //kDebug
    "[INFO]",   //kInfo
    "[NOTICE]",  //kNotice
    "[WARN]",   //kWarn
    "[ERROR]",  //kError
    "[CRIT]",   //kCrit,
    };

Logger::Logger()
    : boost::serialization::singleton<Logger>(),
      emitters_() {
}

Logger::~Logger() {
}
