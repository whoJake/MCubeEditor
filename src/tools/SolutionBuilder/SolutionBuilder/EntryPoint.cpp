#include <iostream>
#include <string>
#include <algorithm>

#include "PremakeBuilder.h"

int main()
{
    std::string filepath;
    std::string version;

    std::cout << "Full filepath: " << std::endl;
    std::getline(std::cin, filepath);
    filepath.erase(std::remove(filepath.begin(), filepath.end(), '"'), filepath.end());

    std::cout << "Version: " << std::endl;
    std::getline(std::cin, version);

    constexpr char* executable = "D:\\Projects\\MCubeEditor\\src\\3rdparty\\premake\\premake5.exe";

    PremakeBuilder builder(executable, filepath.c_str());

    if(strcmp(version.c_str(), "VisualStudio2022") == 0)
    {
        int retval = builder.build(SolutionVersion::VisualStudio2022);
        std::cin.ignore();
        return retval;
    }

    std::cout << "Arguments were incorrect.";
    return -1;
}