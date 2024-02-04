#pragma once

#include "LogTarget.h"

namespace jclog
{

class Log
{
public:
    Log():
        m_targets()
    { };
    ~Log()
    { 
        for( LogTarget* target : m_targets )
        {
            delete target;
        }
    }
    Log(Log&&) = delete;
    Log(const Log&) = delete;

    void register_target(LogTarget* target)
    {
        m_targets.push_back(target);
    }

    template<typename... A>
    void none(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::NONE, functionName, str.c_str());
    }

    template<typename... A>
    void trace(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::TRACE, functionName, str.c_str());
    }

    template<typename... A>
    void track(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::TRACK, functionName, str.c_str());
    }

    template<typename... A>
    void debug(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::DEBUG, functionName, str.c_str());
    }

    template<typename... A>
    void profile(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::PROFILE, functionName, str.c_str());
    }

    template<typename... A>
    void info(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::INFO, functionName, str.c_str());
    }

    template<typename... A>
    void warn(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::WARN, functionName, str.c_str());
    }

    template<typename... A>
    void error(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::ERROR, functionName, str.c_str());
    }

    template<typename... A>
    void event(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::EVENT, functionName, str.c_str());
    }

    template<typename... A>
    void exception(const char* functionName, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::EXCEPTION, functionName, str.c_str());
    }

    template<typename... A>
    void exception(const char* functionName, std::exception exception, const char* fmt, A&&... args) const
    {
        std::string str = std::vformat(fmt, std::make_format_args(args...));
        log(Level::NONE, functionName, exception, str.c_str());
    }

    void log(Level level, const char* functionName, const char* logstr) const
    {
        for( const auto target : m_targets )
        {
            target->log(level, functionName, logstr);
        }
    }

    void log(Level level, const char* functionName, std::exception ex, const char* logstr) const
    {
        for( const auto target : m_targets )
        {
            target->log(level, functionName, ex, logstr);
        }
    }

private:
    std::vector<LogTarget*> m_targets;
};

} // jclog