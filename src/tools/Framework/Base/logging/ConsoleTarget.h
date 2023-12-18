#pragma once
#include "LogTarget.h"

namespace jclog
{

class ConsoleTarget: public LogTarget
{
public:
    ConsoleTarget(bool hasTimestamp = true, const char* timestampFormat = "%H:%M:%S");
    //ConsoleLog(bool hasTimestamp = true, const char* timestampFormat = "%Y-%m-%d %H:%M:%S");
    ~ConsoleTarget();
    ConsoleTarget(ConsoleTarget&&) = delete;
    ConsoleTarget(const ConsoleTarget&) = delete;

protected:
    void log(Level level, const char* functionName, const char* logstr);
    void log(Level level, const char* functionName, std::exception exception, const char* logstr);

private:
    bool m_hasTimestamp;
    const char* m_timestampFormat;
    uint32_t m_padWidth = 0;
};

} // jclog