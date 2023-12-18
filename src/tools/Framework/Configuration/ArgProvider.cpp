#include "ArgProvider.h"
#include <sstream>
#include <format>

static bool is_argument(const std::string& arg)
{
    return arg.starts_with("--");
}

ArgProvider::ArgProvider():
    m_argMap()
{ }

void ArgProvider::register_option(Option option)
{
    // NEED COMPARE TODO:
    bool exists = m_registeredOptions.find(option) != m_registeredOptions.end();

    if( exists )
    {
        std::string exFmt = std::format("Trying to register command option {} but it has already been registered somewhere else.", option.argument);
        throw new std::exception(exFmt.c_str());
    }

    // Little validation on option itself
    if( option.urgency & OptionFlagBits::OPTIONS_VARIABLE
       && option.optionCount == 0 )
    {
        std::string exFmt = std::format("You cannot register a command option that has variable option length but an option count of 0.");
        throw new std::exception(exFmt.c_str());
    }

    m_registeredOptions.insert(option);
}

void ArgProvider::register_option(const std::vector<Option>& options)
{
    for( Option option : options )
    {
        register_option(option);
    }
}

void ArgProvider::validate_option(std::string argument, const std::vector<std::string>& options) const
{
    // Check arg exists in registered arguments
    for( auto it = m_registeredOptions.cbegin(); it != m_registeredOptions.cend(); it++ )
    {
        Option registeredOption = (*it);
        if( registeredOption.argument == argument )
        {
            // Validate here
            int minOptions = 0;
            if( registeredOption.urgency & OptionFlagBits::OPTIONS_OPTIONAL )
                minOptions = 0;
            if( registeredOption.urgency & OptionFlagBits::OPTIONS_VARIABLE )
                minOptions = 1;
            if( registeredOption.urgency & OptionFlagBits::OPTIONS_REQUIRED )
                minOptions = registeredOption.optionCount;

            if( options.size() <= minOptions )
                throw new std::exception(std::format("Command option {} has {} options when it requires at least {}.", argument, options.size(), minOptions).c_str());
            else
                return;
        }
    }

    throw new std::exception(std::format("Command option {} has not been registered.", argument).c_str());
}

void ArgProvider::assign_arguments(const std::vector<std::string>& cmdLine)
{
    m_executable = cmdLine.front();
    std::vector<std::string> argList = std::vector<std::string>(++cmdLine.begin(), cmdLine.end());

    std::string argument;
    std::vector<std::string> options;

    for( std::string arg : argList )
    {
        if( is_argument(arg) )
        {
            // Will throw here
            validate_option(argument, options);
            m_argMap.insert(std::pair(argument, options));

            // Flush current arg/options
            options.clear();
            argument = arg;
            continue;
        }

        if( argument.empty() )
        {
            // Invalid arg list as option isn't assigned to any argument
            throw new std::exception("Command line arguments are invalid. Arguments must be preceeded by \"--\"");
        }

        options.push_back(arg);
    }
}

bool ArgProvider::has_argument(const std::string& argument) const
{
    return m_argMap.find(argument) == m_argMap.end();
}

std::vector<std::string> ArgProvider::get_options(const std::string& argument) const
{
    const auto& itFind = m_argMap.find(argument);
    if (itFind == m_argMap.end()) {
        std::stringstream ss;
        ss << "Argument " << argument << " does not exist.";

        throw std::exception(ss.str().c_str());
    }

    return (*itFind).second;
}

bool ArgProvider::try_get_options(const std::string& argument, std::vector<std::string>* options) const
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

const std::string& ArgProvider::get_executable_path() const
{
    return m_executable;
}
