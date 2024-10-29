#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "operations/base_operation.hpp"
#include "fragment.hpp"

class SCqBaseOperation;
class SCqFragment;

class SCqQuery
{
public:
    SCqQuery(std::vector<std::shared_ptr<SCqBaseOperation>> const & _operations, std::unordered_map<std::string, SCqFragment> const & _fragments):
        operations(_operations), fragments(_fragments){};

    std::vector<std::shared_ptr<SCqBaseOperation>> const & GetOperations() const;
    std::unordered_map<std::string, SCqFragment> const & GetFragments() const;

private:
    std::vector<std::shared_ptr<SCqBaseOperation>> operations;
    std::unordered_map<std::string, SCqFragment> fragments;
};