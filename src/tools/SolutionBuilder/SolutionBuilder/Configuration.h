#pragma once

#include <string>
#include <filesystem>

struct config
{
	std::string executable;
	std::string entry_file;
	std::string version;
};

void setup_configuration_sequence();
void display_configuration_sequence();

const std::filesystem::path& get_configuration_filepath();
bool has_configuration_file();
void create_configuration_file(const config& configuration);

bool get_configuration(config* output);