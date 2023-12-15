#pragma once

#include <exception>
#include <cstdarg>
#include <format>
#include <string>

namespace jclog
{

#define LOG_COLOR_RESET "\033[0m"
#define LOG_COLOR_BOLD "\033[1m"
#define LOG_COLOR_GRAY "\033\30"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE "\033[34m"
#define LOG_COLOR_MAGENTA "\033[35m"
#define LOG_COLOR_CYAN "\033[36m"
#define LOG_COLOR_WHITE "\033[37m"

enum class Level
{
    NONE,
    TRACE,
    DEBUG,
    PROFILE,
    INFO,
    WARN,
    ERROR,
    EVENT,
    EXCEPTION
};

static constexpr const char* level_prefix(Level level)
{
    switch( level )
    {
    case Level::NONE:
        return "     ";
    case Level::TRACE:
        return "trace";
    case Level::DEBUG:
        return "debug";
    case Level::PROFILE:
        return "prof ";
    case Level::INFO:
        return "info ";
    case Level::WARN:
        return "warn ";
    case Level::ERROR:
        return "error";
    case Level::EVENT:
        return "event";
    case Level::EXCEPTION:
        return "ex   ";
    default:
        return "     ";
    }
}

class Log
{
public:
    Log() { };
    virtual ~Log() { }
    Log(Log&&) = delete;
    Log(const Log&) = delete;

    template<typename... A>
    void none(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::NONE, functionName, str.c_str());
    }

    template<typename... A>
    void trace(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::TRACE, functionName, str.c_str());
    }

    template<typename... A>
    void debug(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::DEBUG, functionName, str.c_str());
    }

    template<typename... A>
    void profile(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::PROFILE, functionName, str.c_str());
    }

    template<typename... A>
    void info(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::INFO, functionName, str.c_str());
    }

    template<typename... A>
    void warn(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::WARN, functionName, str.c_str());
    }

    template<typename... A>
    void error(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::ERROR, functionName, str.c_str());
    }

    template<typename... A>
    void event(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::EVENT, functionName, str.c_str());
    }

    template<typename... A>
    void exception(const char* functionName, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::EXCEPTION, functionName, str.c_str());
    }

    template<typename... A>
    void exception(const char* functionName, std::exception exception, const char* fmt, A&&... args)
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::NONE, functionName, exception, str.c_str());
    }

protected:
    virtual void log(Level level, const char* functionName, const char* logstr) = 0;
    virtual void log(Level level, const char* functionName, std::exception exception, const char* logstr) = 0;
};

} // jclog