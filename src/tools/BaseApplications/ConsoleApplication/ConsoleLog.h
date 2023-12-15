#pragma once
#include "Log.h"

namespace jclog
{

class ConsoleLog: public Log
{
public:
    ConsoleLog(bool hasTimestamp = true, const char* timestampFormat = "%H:%M:%S");
    //ConsoleLog(bool hasTimestamp = true, const char* timestampFormat = "%Y-%m-%d %H:%M:%S");
    ~ConsoleLog();
    ConsoleLog(ConsoleLog&&) = delete;
    ConsoleLog(const ConsoleLog&) = delete;

protected:
    void log(Level level, const char* functionName, const char* logstr);
    void log(Level level, const char* functionName, std::exception exception, const char* logstr);

private:
    bool m_hasTimestamp;
    const char* m_timestampFormat;
    uint32_t m_padWidth = 0;
};

} // jclog