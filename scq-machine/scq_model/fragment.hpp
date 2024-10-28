#pragma once

#include <string>
#include <vector>
#include <memory>

#include <field.hpp>

class SCqFragment
{
private:
    std::string name;
    std::vector<std::shared_ptr<SCqField>> fields;
};