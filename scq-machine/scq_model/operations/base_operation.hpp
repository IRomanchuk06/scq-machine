#pragma once

#include <string>
#include <vector>
#include <memory>

#include <field.hpp>

class SCqBaseOperation
{
private:
    enum class OperationType { Query, Mutation, Subscription };
    OperationType type;

    std::string name;
    std::vector<std::shared_ptr<SCqField>> fields;
};