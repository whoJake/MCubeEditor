#include "Config.h"

Config::Config():
    m_argProvider(),
    m_environment()
{ }

Config::~Config()
{ }

ArgProvider& Config::get_arg_provider()
{
    return m_argProvider;
}


Environment& Config::get_environment()
{
    return m_environment;
}
