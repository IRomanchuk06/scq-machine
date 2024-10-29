#include <fragment.hpp>

std::string const &SCqFragment::GetName() const
{
    return name;
}

std::vector<std::shared_ptr<SCqField>> const &SCqFragment::GetFields()
{
    return fields;
}
