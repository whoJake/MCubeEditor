#pragma once
#include <string>
#include <sstream>
#include <iostream>

enum class SolutionVersion
{
    VisualStudio2022,
};

class SolutionBuilder
{
public:
    SolutionBuilder() = delete;
    SolutionBuilder(SolutionBuilder&&) = delete;
    SolutionBuilder(const SolutionBuilder&) = delete;

    virtual int build(SolutionVersion version) const = 0;
protected:
    SolutionBuilder(const std::string& executable) :
        m_Executable(executable)
    { }

    int execute(const std::string& arguments) const {
        std::stringstream cmdLine;
        cmdLine << m_Executable << " " << arguments;
        std::cout << "Running command line:" << std::endl;
        std::cout << cmdLine.str().c_str() << std::endl;

        return system(cmdLine.str().c_str());
    }

private:
    std::string m_Executable;
};