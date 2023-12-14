#pragma once

#include <exception>
#include <cstdarg>

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

    void none(const char* functionName, const char* fmt, ...);
    void trace(const char* functionName, const char* fmt, ...);
    void debug(const char* functionName, const char* fmt, ...);
    void profile(const char* functionName, const char* fmt, ...);
    void info(const char* functionName, const char* fmt, ...);
    void warn(const char* functionName, const char* fmt, ...);
    void error(const char* functionName, const char* fmt, ...);
    void event(const char* functionName, const char* fmt, ...);
    void exception(const char* functionName, const char* fmt, ...);
    void exception(const char* functionName, std::exception exception, const char* fmt, ...);

protected:
    virtual void log(Level level, const char* functionName, const char* fmt, va_list args) = 0;
    virtual void log(Level level, const char* functionName, std::exception exception, const char* fmt, va_list args) = 0;
};

class ConsoleLog : public Log
{
public:
    ConsoleLog(bool hasTimestamp = true, const char* timestampFormat = "%H:%M:%S");
    //ConsoleLog(bool hasTimestamp = true, const char* timestampFormat = "%Y-%m-%d %H:%M:%S");
    ~ConsoleLog();
    ConsoleLog(ConsoleLog&&) = delete;
    ConsoleLog(const ConsoleLog&) = delete;

protected:
    void log(Level level, const char* functionName, const char* fmt, va_list args);
    void log(Level level, const char* functionName, std::exception exception, const char* fmt, va_list args);

private:
    bool m_HasTimestamp;
    const char* m_TimestampFormat;
    uint32_t m_PadWidth = 0;
};

class FileLog
{
public:
    FileLog();
    ~FileLog();
    FileLog(FileLog&&) = delete;
    FileLog(const FileLog&) = delete;

protected:
    void log(Level level, const char* functionName, const char* fmt, va_list args);
    void log(Level level, const char* functionName, std::exception exception, const char* fmt, va_list args) ;
};

} // jclog