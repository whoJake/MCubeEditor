#include "CommandArgs.h"
#include <sstream>

CommandArgs::CommandArgs(const std::vector<std::string>& argList)
{

}

bool CommandArgs::has_argument(const std::string& argument) const
{
    return m_ArgMap.find(argument) == m_ArgMap.end();
}

std::vector<std::string> CommandArgs::get_options(const std::string& argument) const
{
    auto& itFind = m_ArgMap.find(argument);
    if (itFind == m_ArgMap.end()) {
        std::stringstream ss;
        ss << "Argument " << argument << " does not exist.";

        throw std::exception(ss.str().c_str());
    }

    return (*itFind).second;
}

bool CommandArgs::try_get_options(const std::string& argument, std::vector<std::string>* options) const
{
    try
    {
        *options = get_options(argument);
        return true;
    }
    catch( std::exception ex )
    {
        return false;
    }
}