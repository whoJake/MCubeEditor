#pragma once

#include "LogTarget.h"

namespace jclog
{

class EmptyTarget : public LogTarget
{
public:
    EmptyTarget() = default;
    
    EmptyTarget(EmptyTarget&&) = delete;
    EmptyTarget(const EmptyTarget&) = delete;
protected:
    void log(Level level, const char* functionName, const char* logstr) const { }
    void log(Level, const char* functionName, std::exception exception, const char* logstr) const { }
};

}