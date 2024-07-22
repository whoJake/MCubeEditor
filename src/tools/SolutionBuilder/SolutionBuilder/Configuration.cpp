#include "Configuration.h"
#include <iostream>
#include <fstream>

std::filesystem::path* config_cache = nullptr;

void setup_configuration_sequence()
{
	config configuration{ };

	{
		std::cout << "Enter executable filepath:\n\t";
		std::string executable;
		std::getline(std::cin, executable);
		executable.erase(std::remove(executable.begin(), executable.end(), '"'), executable.end());
		configuration.executable = executable;
	}

	{
		std::cout << "\nEnter entry file:\n\t";
		std::string entry;
		std::getline(std::cin, entry);
		entry.erase(std::remove(entry.begin(), entry.end(), '"'), entry.end());
		configuration.entry_file = entry;
	}

	{
		std::cout << "\nEnter version:\n\t";
		std::string version;
		std::getline(std::cin, version);
		version.erase(std::remove(version.begin(), version.end(), '"'), version.end());
		configuration.version = version;
	}

	std::cout << "\n";
	create_configuration_file(configuration);
}

void display_configuration_sequence()
{
	config configuration{ };
	bool exists = get_configuration(&configuration);
	if( !exists )
	{
		std::cout << "No configuration file to display.\n";
		return;
	}

	std::cout << "Active Configuration\n";
	std::cout << "\tExecutable: " << configuration.executable << '\n';
	std::cout << "\tEntry File: " << configuration.entry_file << '\n';
	std::cout << "\tVersion: " << configuration.version << '\n';
}

const std::filesystem::path& get_configuration_filepath()
{
	if (config_cache)
		return *config_cache;

	constexpr const char* localpath = "Personal\\.sbconfig";
	std::filesystem::path local = std::getenv("localappdata");
	config_cache = new std::filesystem::path(local.append(localpath));
	return *config_cache;
}

bool has_configuration_file()
{
	return std::ifstream(get_configuration_filepath().c_str()).good();
}

void create_configuration_file(const config& configuration)
{
	const std::filesystem::path& fileLocation = get_configuration_filepath();
	std::filesystem::create_directory(fileLocation.parent_path());

	std::ofstream file(fileLocation, std::ios::out | std::ios::trunc | std::ios::binary);

	file << configuration.executable << '\n';
	file << configuration.entry_file << '\n';
	file << configuration.version << '\n';
}

bool get_configuration(config* output)
{
	if( !output )
		return false;

	if( !has_configuration_file() )
		return false;

	std::ifstream file(get_configuration_filepath(), std::ios::in | std::ios::binary);
	if( !file.is_open() )
		return false;

	config out{ };
	std::getline(file, out.executable);
	std::getline(file, out.entry_file);
	std::getline(file, out.version);

	if (out.executable.empty()
		|| out.entry_file.empty()
		|| out.version.empty())
	{
		return false;
	}

	*output = out;
	
	return true;
}