#include <directive.hpp>

std::string const& SCqDirective::GetName() const {
    return name;
}

std::unordered_map<std::string, std::string> const& SCqDirective::GetArguments() const {
    return arguments;
}