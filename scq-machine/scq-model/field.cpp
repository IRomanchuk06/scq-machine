#include "field.hpp"

std::string const& SCqField::GetName() const {
    return name;
}

std::optional<std::string> const& SCqField::GetAlias() const {
    return alias;
}

std::vector<SCqArgument> const& SCqField::GetArguments() const {
    return arguments;
}

std::vector<SCqDirective> const& SCqField::GetDirectives() const {
    return directives;
}

std::vector<std::shared_ptr<SCqField>> const& SCqField::GetSubfields() const {
    return subfields;
}