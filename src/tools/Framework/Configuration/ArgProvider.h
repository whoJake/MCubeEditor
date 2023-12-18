#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

enum OptionFlagBits: uint32_t
{
    ARGUMENT_OPTIONAL,
    OPTIONS_OPTIONAL,
    OPTIONS_VARIABLE,
    OPTIONS_REQUIRED,
};

typedef uint32_t OptionFlags;

struct Option
{
    const char* argument;
    int optionCount;
    OptionFlags urgency;
};

static bool operator==(const Option& left, const Option& right)
{
    return strcmp(left.argument, right.argument) == 0;
}

template<>
struct std::hash<Option>
{
    size_t operator()(const Option& option) const {
        std::hash<std::string> hasher;
        return hasher(std::string(option.argument));
    }
};

class ArgProvider
{
public:
    ArgProvider();
    ~ArgProvider() { }

    void register_option(Option option);
    void register_option(const std::vector<Option>& options);

    void assign_arguments(const std::vector<std::string>& argList);
    bool has_argument(const std::string& argument) const;
    std::vector<std::string> get_options(const std::string& argument) const;

    bool try_get_options(const std::string& argument, std::vector<std::string>* options) const;

    const std::string& get_executable_path() const;
private:
    std::string m_executable;
    std::unordered_set<Option> m_registeredOptions;
    std::unordered_map<std::string, std::vector<std::string>> m_argMap;

    void validate_option(std::string argument, const std::vector<std::string>& options) const;
};