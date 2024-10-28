#pragma once

#include <optional>

#include <argument.hpp>
#include <directive.hpp>

class SCqField
{
private:
    std::string name;
    std::optional<std::string> alias;
    std::vector<SCqArgument> arguments;
    std::vector<SCqDirective> directives;
    std::vector<std::shared_ptr<SCqField>> subfields;
};