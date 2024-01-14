#include <exception>
#include <cstdarg>
#include <format>

class FormattedException : public std::exception {
public:
    template<typename ... A>
    FormattedException(const char* fmt, A&&... args) :
        std::exception(std::vformat(fmt, std::make_format_args(args...)).c_str())
    { }

};

class VulkanException : public FormattedException {
public:
    template<typename ... A>
    VulkanException(const char* fmt, A&&... args) :
        FormattedException(fmt, std::forward<A>(args)...)
    { }
};