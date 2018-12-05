#ifndef LOG_H
#define LOG_H

#include <string>

#define MESSAGE_MAX_LEN 255

class Log
{
public:
    enum ELogLevel
    {
        LOG_VERBOSE = 0,
        LOG_INFO = 1,
        LOG_WARNING = 2,
        LOG_ERROR = 3,
        LOG_NONE = 4,
    };
    static void set_level(ELogLevel level);
    static void message(ELogLevel level, std::string fmt, ...);
private:
    static ELogLevel log_level;
};

#endif // LOG_H
