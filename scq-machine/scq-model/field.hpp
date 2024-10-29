#pragma once

#include <optional>
#include <vector>
#include <memory>

#include "argument.hpp"
#include "directive.hpp"

class SCqArgument;
class SCqDirective;

class SCqField
{
public:
    SCqField(std::string const& _name,
             std::optional<std::string> const& _alias,
             std::vector<SCqArgument> const& _arguments,
             std::vector<SCqDirective> const& _directives,
             std::vector<std::shared_ptr<SCqField>> const& _subfields):
                name(_name), alias(_alias), arguments(_arguments), directives(_directives), subfields(_subfields) {}

    std::string const& GetName() const;
    std::optional<std::string> const& GetAlias() const;
    std::vector<SCqArgument> const& GetArguments() const;
    std::vector<SCqDirective> const& GetDirectives() const;
    std::vector<std::shared_ptr<SCqField>> const& GetSubfields() const;

private:
    std::string name;
    std::optional<std::string> alias;
    std::vector<SCqArgument> arguments;
    std::vector<SCqDirective> directives;
    std::vector<std::shared_ptr<SCqField>> subfields;
};