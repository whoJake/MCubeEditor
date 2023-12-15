#pragma once

#include "Log.h"
#include <fstream>

namespace jclog
{

class FileLog: public Log
{
public:
    FileLog(const char* logDirectory);
    ~FileLog();
    FileLog(FileLog&&) = delete;
    FileLog(const FileLog&) = delete;

protected:
    void log(Level level, const char* functionName, const char* logstr);
    void log(Level level, const char* functionName, std::exception exception, const char* logstr);

private:
    std::fstream m_logFile;
};

}