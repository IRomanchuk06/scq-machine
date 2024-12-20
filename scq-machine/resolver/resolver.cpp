#include <resolver.hpp>

using namespace SCqResolverConstants;

ScAgentContext * SCqResolver::m_context = nullptr;

std::string SCqResolver::Resolve()
{
    std::string result = "";
    result += OPEN_CURLY_BRACKETS + END_OF_LINE;

    for(auto const & operation : root->children)
    {
        SCqNodeType operationType = operation->type;

        std::string operationResult;

        switch(operationType)
        {
        case SCqNodeType::Query:
            operationResult = LaunchQueryAgent(operation);
            break;

        case SCqNodeType::Mutation:
            operationResult = LaunchMutationAgent(operation);
            break;

        case SCqNodeType::Subscription:
            operationResult = LaunchSubscriptionAgent(operation);
            break;
        
        default:
            break;
        }

        result += AddTabToEachLine(operationResult);
    }

    result += CLOSE_CURLY_BRACKETS;

    return result;
}

std::string SCqResolver::LaunchQueryAgent(std::shared_ptr<SCqNode> operationRoot)
{
    auto query = std::static_pointer_cast<SCqQuery>(operationRoot);

    std::string queryResult = "";
    queryResult += QUOTES + query->GetOperationName() + UNDERLINE + query->value + QUOTES + SPACE + COLON + SPACE + OPEN_CURLY_BRACKETS + END_OF_LINE;

    SCqQuery::SCqQueryType queryType = query->operationType;
    
    auto actionClass = SCqResolverKeynodes::action_query_related_entities;

    std::vector<ScAddr> actionQueryEntities;

    for(auto const & entity: operationRoot->children)
    {
        ScAddr const & argsStruct = CreateArgsStruct(operationRoot->children[0]);
        ScAddr const & entityAddr = m_context->SearchElementBySystemIdentifier(entity->value);

        ScAction actionQuery = m_context->GenerateAction(SCqResolverKeynodes::action_query_related_entities);
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

std::string SCqResolver::LaunchMutationAgent(std::shared_ptr<SCqNode> operationRoot)
{
    auto mutation = std::static_pointer_cast<SCqMutation>(operationRoot);

    SCqMutation::SCqMutationType mutationType = mutation->operationType;

    switch (mutationType)
    {
    case SCqMutation::SCqMutationType::MutationRelatedEntities:
        CreateArgsStruct(operationRoot->children[0]);
        break;
    
    default:
        break;
    }
}

std::string SCqResolver::LaunchSubscriptionAgent(std::shared_ptr<SCqNode> operationRoot)
{
    auto subscription = std::static_pointer_cast<SCqSubscription>(operationRoot);
}

std::string SCqResolver::GenerateJSON(ScAddr const &actionAddr)
{
    ScAction actionJSON = m_context->GenerateAction(SCqResolverKeynodes::action_generate_json);
    actionJSON.SetArguments(actionAddr);
    actionJSON.InitiateAndWait();

    auto linkStream = m_context->GetLinkContent(actionJSON.GetResult()); 

    ScStreamConverter converter;

    std::string result;
    converter.StreamToString(linkStream, result);

    return result;
}

ScAddr const &SCqResolver::CreateArgsStruct(std::shared_ptr<SCqNode> entity)
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

std::string SCqResolver::AddTabToEachLine(const std::string &input)
{
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;

    while (std::getline(iss, line)) {
        oss << '\t' << line << '\n';
    }

    return oss.str();
}