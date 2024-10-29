#pragma once

#include <string>
#include <vector>
#include <memory>

#include "field.hpp"

class SCqField;

class SCqFragment
{
public:
    SCqFragment(std::string const & _name,
                std::vector<std::shared_ptr<SCqField>> const & _fields):
                                      name(_name), fields(_fields){};

    std::string const & GetName() const;
    std::vector<std::shared_ptr<SCqField>> const & GetFields();

private:
    std::string name;
    std::vector<std::shared_ptr<SCqField>> fields;
};