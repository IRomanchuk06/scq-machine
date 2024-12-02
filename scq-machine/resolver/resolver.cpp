#include <resolver.hpp>

#include <sc-memory/sc_agent_context.hpp>

// rude example

std::string SCqResolver::Resolve()
{
    auto entity = root->children[0];
    std::string entityName = entity->value;

    std::vector<ScAddr> fieldIdtf;

    ScAgentContext context;

    for(auto const & field: entity->children)
    {
        auto arg = context.SearchElementBySystemIdentifier(field->value);
        fieldIdtf.push_back(arg);
    }
    
    ScAddr const & entityNode = context.SearchElementBySystemIdentifier(entityName);
    ScAddr const & argsStructNode = context.GenerateNode(ScType::NodeConstStruct);

    for (auto const & arg: fieldIdtf)
    {
        context.GenerateConnector(ScType::EdgeAccessConstPosPerm, argsStructNode, arg);
    }

    ScAddr const & actionNode = context.GenerateNode(ScType::NodeConst);
    context.GenerateConnector(ScType::EdgeAccessConstPosPerm, context.SearchElementBySystemIdentifier("action_search_specified_relations"), actionNode);

    ScAddr const & firstArgConnector = context.GenerateConnector(ScType::EdgeAccessConstPosPerm, actionNode, entityNode);
    context.GenerateConnector(ScType::EdgeAccessConstPosPerm, ScKeynodes::rrel_1, firstArgConnector);

    ScAddr const & secondArgConnector = context.GenerateConnector(ScType::EdgeAccessConstPosPerm, actionNode, argsStructNode);
    context.GenerateConnector(ScType::EdgeAccessConstPosPerm, ScKeynodes::rrel_2, secondArgConnector);

    context.GenerateConnector(ScType::EdgeAccessConstPosPerm, ScKeynodes::action_initiated, actionNode);

    
}
