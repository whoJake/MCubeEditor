#pragma once
#include <string>
#include <vector>
#include "CommandArgs.h"

enum ExitFlagBits : int32_t
{
    Success = 0,
    Failure = 1 << 0
};
typedef int32_t ExitFlags;

class ApplicationBase
{
public:
    ApplicationBase();
    virtual ~ApplicationBase();
    ApplicationBase(ApplicationBase&&) = delete;
    ApplicationBase(const ApplicationBase&) = delete;

    virtual ExitFlags run(int argc, const char* argv[]) final;

    virtual ExitFlags app_main() = 0;
    virtual void app_shutdown(ExitFlags exitFlags = 0) final;
    virtual void on_app_shutdown() { }
private:
    ExitFlags m_exitFlags;
    CommandArgs m_commandArgs;
};
