#include "resolver.hpp"
#include "query_resolver.hpp"

using namespace SCqResolverConstants;

std::string SCqQueryResolver::Resolve()
{
    std::string queryResult = "";
    queryResult += QUOTES + queryRoot->GetOperationName() + UNDERLINE + queryRoot->value + QUOTES + SPACE + COLON + SPACE + OPEN_CURLY_BRACKETS + END_OF_LINE;

    SCqQuery::SCqQueryType queryType = queryRoot->operationType;

    ScKeynode actionClass = queryRoot->GetOperationActionClass();

    std::vector<ScAddr> actionQueryEntities;

    for(auto const & entity: queryRoot->children)
    {
        ScAddr const & argsStruct = CreateArgsStruct(queryRoot->children[0]);
        ScAddr const & entityAddr = m_context->SearchElementBySystemIdentifier(entity->value);

        ScAction actionQuery = m_context->GenerateAction(actionClass);
        actionQuery.SetArguments(entityAddr, argsStruct);
        actionQuery.InitiateAndWait();

        actionQueryEntities.push_back(actionQuery);
    }

    for(auto const & action: actionQueryEntities)
    {
        std::string actionRes = GenerateJSON(action);

        queryResult += AddTabToEachLine(actionRes);
    }

    queryResult += END_OF_LINE + CLOSE_CURLY_BRACKETS;

    return queryResult;
}

std::string SCqQueryResolver::GenerateJSON(ScAddr const &actionQuery)
{
    ScAction actionJSON = m_context->GenerateAction(SCqResolverKeynodes::action_generate_json);
    actionJSON.SetArguments(actionQuery);
    actionJSON.InitiateAndWait();

    auto linkStream = m_context->GetLinkContent(actionJSON.GetResult()); 

    ScStreamConverter converter;

    std::string result;
    converter.StreamToString(linkStream, result);

    return result;
}

ScAddr SCqQueryResolver::CreateArgsStruct(std::shared_ptr<SCqNode> entity)
{
    ScAddr const & argsStruct = m_context->GenerateNode(ScType::NodeConstStruct);

    for (auto const & arg: entity->children)
    {
        ScAddr const & argAddr = m_context->SearchElementBySystemIdentifier(arg->value);

        // skip arg list in nested
        if(arg->type == SCqNodeType::Argument)
        {
            continue;
        }

        // all args have children[0] that contains the values of args
        std::vector<std::shared_ptr<SCqNode>> values = arg->children[0]->children;

        if(!values.empty())
        { 
            for(auto const & value: values)
            {
                ScAddr const & valueAddr = m_context->SearchElementBySystemIdentifier(value->value);

                ScAddr const connector = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argAddr, valueAddr);
                m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_argument_value, connector);
            }
        }

        ScAddr const & argEdge = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argsStruct, argAddr);
        m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_argument, argEdge);

        if(arg->children.size() > 1)
        {
            ScAddr const & nestedEdge = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argAddr, CreateArgsStruct(arg));
            m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_nested, nestedEdge);
        }
    }

    return argsStruct;
}
