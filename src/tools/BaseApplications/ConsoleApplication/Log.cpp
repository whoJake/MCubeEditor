#include "jclog.h"

namespace jclog
{

void Log::none(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::NONE, functionName, fmt, args);
    va_end(args);
}

void Log::trace(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::TRACE, functionName, fmt, args);
    va_end(args);
}

void Log::debug(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::DEBUG, functionName, fmt, args);
    va_end(args);
}

void Log::profile(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::PROFILE, functionName, fmt, args);
    va_end(args);
}

void Log::info(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::INFO, functionName, fmt, args);
    va_end(args);
}

void Log::warn(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::WARN, functionName, fmt, args);
    va_end(args);
}

void Log::error(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::ERROR, functionName, fmt, args);
    va_end(args);
}

void Log::event(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::EVENT, functionName, fmt, args);
    va_end(args);
}

void Log::exception(const char* functionName, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::EXCEPTION, functionName, fmt, args);
    va_end(args);
}

void Log::exception(const char* functionName, std::exception exception, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(Level::EXCEPTION, functionName, exception, fmt, args);
    va_end(args);
}

} // jclog