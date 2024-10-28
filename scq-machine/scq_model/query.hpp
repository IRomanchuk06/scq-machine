#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include <base_operation.hpp>
#include <fragment.hpp>

class SCqQuery
{
private:
    std::vector<std::shared_ptr<SCqBaseOperation>> operations;
    std::unordered_map<std::string, SCqFragment> fragments;
};