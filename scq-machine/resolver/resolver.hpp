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
        root(_root)
        {
            if (m_context == nullptr)
                m_context = new ScAgentContext();
        };

    virtual std::string Resolve();

    // These functions are overridden in derived classes, but are defined here in the base class
    // to allow for the creation of an instance of the base class.
    virtual std::string GenerateJSON(ScAddr const& actionQuery) {return "";};
    virtual ScAddr CreateArgsStruct(std::shared_ptr<SCqNode> entity) {return 0;}; 

protected:
    std::shared_ptr<SCqNode> const root;

    static ScAgentContext * m_context;
    static std::string AddTabToEachLine(const std::string &input);   
};

#include "query_resolver.hpp"
#include "mutation_resolver.hpp"