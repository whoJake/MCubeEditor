#pragma once
#include "ArgProvider.h"

class Config
{
public:
    Config();
    ~Config();

    ArgProvider& get_arg_provider();
    //Environment& get_environment();
private:
    ArgProvider m_argProvider;
    //Environment m_environment;
};