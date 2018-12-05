#include "log.h"
#include <stdarg.h>
#include <iostream>

Log::ELogLevel Log::log_level;

void Log::set_level(ELogLevel level)
{
    log_level = level;
}

void Log::message(ELogLevel level, std::string fmt, ...)
{
    if (level < log_level)
        return;

    char text[MESSAGE_MAX_LEN];
    va_list ap;

    if (fmt.length() != 0)
    {
        va_start(ap, fmt);
        _vsnprintf(text, MESSAGE_MAX_LEN, fmt.c_str(), ap);
        va_end(ap);

        std::cout << text;
    }
}

/*void Log::message(std::string msg)
{
    std::cout << msg;
}*/
