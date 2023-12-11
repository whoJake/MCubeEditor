#pragma once
#include <string>
#include <vector>

enum ExitFlagBits : int32_t
{
    Success = 0,
    Failure = 1 << 0
};
typedef int32_t ExitFlags;

class ApplicationBase
{
public:
    ApplicationBase() = delete;
    ~ApplicationBase();
    ApplicationBase(ApplicationBase&&) = delete;
    ApplicationBase(const ApplicationBase&) = delete;

    virtual ExitFlags app_main() final;
    virtual void app_shutdown(ExitFlags exitcode = 0) final;

    virtual ExitFlags on_app_main() { }
    virtual ExitFlags on_app_shutdown() { }
protected:
    ApplicationBase(const std::vector<std::string> argumentList);
private:
    bool m_Running;
    ExitFlags m_Exitflags;
};

#ifndef APP_NO_MAIN
static ApplicationBase* create_application(const std::vector<std::string> args);
#endif