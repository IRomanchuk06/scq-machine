#pragma once

#include <string>
#include <unordered_map>

class SCqDirective
{
public:
    SCqDirective(std::string const& _name, std::unordered_map<std::string, std::string> const& _arguments):
        name(_name), arguments(_arguments){};

    std::string const& GetName() const;
    std::unordered_map<std::string, std::string> const& GetArguments() const;

private:
    std::string name;
    std::unordered_map<std::string, std::string> arguments;
};