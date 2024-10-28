#pragma once

#include <string>
#include <variant>

class SCqArgument
{
private:
    std::string name;
    std::variant<std::string, int, bool> value;
};