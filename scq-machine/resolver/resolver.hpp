#pragma once

#include "parser.hpp"

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

class SCqResolver
{
public:
    SCqResolver(std::shared_ptr<SCqNode> const _root):
        root(_root){};

    std::string Resolve();

private:
    static ScAgentContext m_context;

    std::shared_ptr<SCqNode> const root;

    ScAddr const & LaunchQueryAgent(std::shared_ptr<SCqNode> operationRoot);
    ScAddr const & LaunchMutationAgent(std::shared_ptr<SCqNode> operationRoot);
    ScAddr const & LaunchSubscriptionAgent(std::shared_ptr<SCqNode> operationRoot);

    void PrepareRelationsStructure(ScAddr const &agentArgsStruct, std::vector<std::shared_ptr<SCqNode>> const &args);
    void GenerateRelationArgument(ScAddr const& rel, ScAddr const & arg);
    ScAddr const & CheckArgumentValue(std::shared_ptr<SCqNode> arg);
};