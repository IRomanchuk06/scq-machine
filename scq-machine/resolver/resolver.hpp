#pragma once

#include "parser.hpp"

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

#include "keynodes.hpp"
#include "resolver_constants.hpp"

class SCqResolver
{
public:
    SCqResolver(std::shared_ptr<SCqNode> const _root):
        root(_root){
            if (m_context == nullptr)
                m_context = new ScAgentContext();
        };

    std::string Resolve();

private:
    static ScAgentContext * m_context;

    std::shared_ptr<SCqNode> const root;

    std::string LaunchQueryAgent(std::shared_ptr<SCqNode> operationRoot);
    std::string LaunchMutationAgent(std::shared_ptr<SCqNode> operationRoot);
    std::string LaunchSubscriptionAgent(std::shared_ptr<SCqNode> operationRoot);

    std::string GenerateJSON(ScAddr const& actionQuery);

    ScAddr const & CreateArgsStruct(std::shared_ptr<SCqNode> entity);

    std::string AddTabToEachLine(const std::string &input);
};