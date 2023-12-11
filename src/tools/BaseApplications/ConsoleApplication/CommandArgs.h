#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class CommandArgs
{
public:
    CommandArgs() = delete;
    CommandArgs(const std::vector<std::string>& argList);

    CommandArgs(CommandArgs&&) = delete;
    CommandArgs(const CommandArgs&) = delete;

    bool has_argument(const std::string& argument) const;
    std::vector<std::string> get_options(const std::string& argument) const;

    bool try_get_options(const std::string& argument, std::vector<std::string>* options) const;
private:
    std::unordered_map<std::string, std::vector<std::string>> m_ArgMap;
};