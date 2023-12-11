#pragma once

#include "SolutionBuilder.h"

#include <string>

class PremakeBuilder : public SolutionBuilder
{
public:
    PremakeBuilder(std::string executablePath,
                   std::string premakeFilepath);
    ~PremakeBuilder();

    int build(SolutionVersion version) const override;
private:
    std::string m_Filepath;
};