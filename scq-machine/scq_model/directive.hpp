#pragma once

#include <string>
#include <unordered_map>

class SCqDirective
{
private:
    std::string name;
    std::unordered_map<std::string, std::string> arguments;
};