#pragma once
#include <string>
#include <vector>
#include "configuration/Config.h"
#include "logging/jclog.h"

class Environment;

enum ExitFlagBits : int32_t
{
    Success = 0,
    Failure = 1 << 0,
    InitFailure = 1 << 1
};
typedef int32_t ExitFlags;

class ApplicationBase
{
public:
    ApplicationBase();
    virtual ~ApplicationBase();
    ApplicationBase(ApplicationBase&&) = delete;
    ApplicationBase(const ApplicationBase&) = delete;

    virtual int run(int argc, const char* argv[]) final;

    virtual ExitFlags app_main() = 0;
    virtual void app_shutdown(ExitFlags exitFlags = 0) final;
    virtual void on_app_shutdown() { }

    jclog::Log& get_log();

    Config& get_config();
    Environment& get_environment();
protected:
private:
    ExitFlags m_exitFlags;
    Config m_config;
    jclog::Log m_log;
};
