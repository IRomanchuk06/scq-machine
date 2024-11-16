#pragma once

#include <string>
#include <vector>
#include <memory>

#include "field.hpp"

class SCqField;

class SCqBaseOperation
{
public:
    enum class OperationType { Query, Mutation, Subscription };

    SCqBaseOperation(OperationType _type, std::string const& _name,
                    std::vector<std::shared_ptr<SCqField>> const& _fields):
                        type(_type), name(_name), fields(_fields){};

    OperationType GetType() const;
    std::string const& GetName() const;
    std::vector<std::shared_ptr<SCqField>> const& GetFields() const;

private:
    OperationType type;

    std::string name;
    std::vector<std::shared_ptr<SCqField>> fields;
};