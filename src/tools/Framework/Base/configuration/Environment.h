#pragma once
#include <string>
#include <unordered_map>

class Environment
{
public:
    Environment();
    ~Environment();

    void import_var(const std::string& var, const std::string& replace);
    bool can_subst(const std::string& str) const;
    std::string subst(std::string str) const;

    void clear();
private:
    std::unordered_map<std::string, std::string> m_argMappings;
};