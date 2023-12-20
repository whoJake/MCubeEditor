#include "Environment.h"
#include <format>

Environment::Environment():
    m_argMappings()
{ }

Environment::~Environment()
{
    m_argMappings.clear();
}

void Environment::import_var(const std::string& var, const std::string& replace)
{ 
    m_argMappings[var] = replace;
}

bool Environment::can_subst(const std::string& str) const
{ 
    for( auto it = m_argMappings.cbegin(); it != m_argMappings.cend(); ++it )
    {
        const std::string& var = (*it).first;
        std::string search = std::format("$({})", var);

        if( str.find(search) != str.npos )
            return true;
    }
    return false;
}

std::string Environment::subst(std::string str) const
{ 
    // Just incase hehe.
    int max_iterations = 20;
    int iteration = 0;

    while( can_subst(str) && iteration < max_iterations )
    {
        for( auto it = m_argMappings.begin(); it != m_argMappings.end(); ++it )
        {
            std::string find = std::format("$({})", (*it).first);

            size_t findpos = str.find(find);
            if( findpos != str.npos )
            {
                std::string itermediate = str.erase(findpos, find.length());
                str = itermediate.insert(findpos, (*it).second);
                iteration++;
            }
        }
    }

    return str;
}

void Environment::clear()
{
    m_argMappings.clear();
}

