#include <iostream>
#include <string>
#include <algorithm>

#include "PremakeBuilder.h"
#include "Configuration.h"

void display_valid_options(bool showhint)
{
    if( showhint )
        std::cout << "\tValid Options\n";

    std::cout << "\t[s] setup\n";
    std::cout << "\t[r] replay\n";
    std::cout << "\t[d] display\n";
    std::cout << "\t[e/exit] exit\n";
}

bool build_sequence(config configuration)
{
    PremakeBuilder builder(configuration.executable, configuration.entry_file);
    int exitcode = 0;

    if (configuration.version == "vs2022"
        || configuration.version == "VS2022"
        || configuration.version == "VisualStudio2022"
        || configuration.version == "visualstudio2022")
    {
        exitcode = builder.build(SolutionVersion::VisualStudio2022);
    }
    else
    {
        exitcode = 1;
    }

    return exitcode == 0;
}

int main()
{
    bool running = true;

    while(running)
    {
        if( !has_configuration_file() )
        {
            setup_configuration_sequence();
            continue;
        }

        std::string option;
        std::cout << "Enter option:\n";

        bool requireHint = false;
        while( option.empty() )
        {
            display_valid_options(requireHint);
            requireHint = true;
            std::getline(std::cin, option);
            
            if( option == "s" )
            {
                std::cout << '\n';
                setup_configuration_sequence();
            }
            else if( option == "r" )
            {
                config settings{ };
                if( get_configuration(&settings) )
                {
                    bool success = build_sequence(settings);
                    std::cout << '\n';
                    if( !success )
                    {
                        std::cout << "Configuration was invalid\n";
                        setup_configuration_sequence();
                    }
                }
                else
                {
                    std::cout << "Failed to load configuration.\n";
                    setup_configuration_sequence();
                }
            }
            else if( option == "d" )
            {
                std::cout << '\n';
                display_configuration_sequence();
            }
            else if( option == "e"
                || option == "exit" )
            {
                running = false;
            }
            else
                option = "";
        }
    }

    return 0;
}