#include <argument.hpp>

std::string const& SCqArgument::GetName() const {
    return name;
}

std::variant<std::string, int, bool> const& SCqArgument::GetValue() const {
    return value;
}