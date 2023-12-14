#include "jclog.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace jclog
{

static void pad_stringstream(std::stringstream* ss, uint32_t padWidth)
{
    for( uint32_t i = 0; i < padWidth; i++ )
        *ss << " ";
}

ConsoleLog::ConsoleLog(bool hasTimestamp, const char* timestampFormat):
    m_HasTimestamp(hasTimestamp),
    m_TimestampFormat(timestampFormat)
{ 
    m_PadWidth += 8; // [{5 spaces}]{1 space}
    if( m_HasTimestamp )
        m_PadWidth += strlen(timestampFormat) + 3; // []{1 space}
}

ConsoleLog::~ConsoleLog()
{ }

void ConsoleLog::log(Level level, const char* functionName, const char* fmt, va_list args)
{
    if( m_HasTimestamp )
    {
        time_t time = std::time(nullptr);
        std::tm timeStruct = *std::localtime(&time);

        std::cout << "[" << std::put_time(&timeStruct, m_TimestampFormat) << "] ";
    }

    std::cout << "[";

    switch( level )
    {
    case Level::TRACE:
        std::cout << LOG_COLOR_GRAY;
        break;
    case Level::DEBUG:
        std::cout << LOG_COLOR_CYAN;
        break;
    case Level::PROFILE:
        std::cout << LOG_COLOR_BLUE;
        break;
    case Level::INFO:
        std::cout << LOG_COLOR_YELLOW;
        break;
    case Level::WARN:
        std::cout << LOG_COLOR_BOLD << LOG_COLOR_YELLOW;
        break;
    case Level::ERROR:
        std::cout << LOG_COLOR_RED;
        break;
    case Level::EVENT:
        std::cout << LOG_COLOR_GREEN;
        break;
    case Level::EXCEPTION:
        std::cout << LOG_COLOR_BOLD << LOG_COLOR_RED;
        break;
    default:
        std::cout << LOG_COLOR_WHITE;
        break;
    }

    std::cout << level_prefix(level) << LOG_COLOR_RESET << "] ";

    vprintf(fmt, args);
    std::cout << std::endl;
}

void ConsoleLog::log(Level level, const char* functionName, std::exception exception, const char* fmt, va_list args)
{
    log(level, functionName, fmt, args);

    std::stringstream ss;
    ss << "Exception:\n";
    pad_stringstream(&ss, m_PadWidth);
    ss << "\t%s";

    // Exception formatting?
    this->none(functionName, ss.str().c_str(), exception.what());
}

} // jclog