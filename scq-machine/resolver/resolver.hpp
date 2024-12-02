#pragma once

#include "parser.hpp"

#include <sc-memory/sc_memory.hpp>

class SCqResolver
{
public:
    SCqResolver(std::shared_ptr<SCqNode> const _root):
        root(_root){};

    std::string Resolve();

private:
    std::shared_ptr<SCqNode> const root;

    ScAddr const & LaunchQueryAgent(std::shared_ptr<SCqNode> operationRoot);
    ScAddr const & LaunchMutationAgent(std::shared_ptr<SCqNode> operationRoot);
    ScAddr const & LaunchSubscriptionAgent(std::shared_ptr<SCqNode> operationRoot);
};