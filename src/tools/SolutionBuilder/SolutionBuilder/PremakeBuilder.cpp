#include "PremakeBuilder.h"

#include <sstream>

PremakeBuilder::PremakeBuilder(std::string executablePath,
                               std::string filepath) :
    SolutionBuilder(executablePath),
    m_Filepath(filepath)
{ }

PremakeBuilder::~PremakeBuilder()
{ }

int PremakeBuilder::build(SolutionVersion version) const {
    std::stringstream arguments;

    arguments << " --file=" << m_Filepath << " ";

    switch(version)
    {
    case SolutionVersion::VisualStudio2022:
        arguments << "vs2022";
        break;
    default:
        break;
    }

    return execute(arguments.str());
}