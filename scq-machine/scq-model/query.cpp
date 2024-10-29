#include <query.hpp>

std::vector<std::shared_ptr<SCqBaseOperation>> const & SCqQuery::GetOperations() const
{
    return operations;
}

std::unordered_map<std::string, SCqFragment> const & SCqQuery::GetFragments() const 
{
    return fragments;
}
