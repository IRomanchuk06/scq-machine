#pragma once

#include <string>
#include <variant>

class SCqArgument
{
public:
    SCqArgument(std::string const& _name, std::variant<std::string, int, bool> const& _value):
        name(_name), value(_value){};

    std::string const& GetName() const;
    std::variant<std::string, int, bool> const& GetValue() const;

private:
    std::string name;
    std::variant<std::string, int, bool> value;
};