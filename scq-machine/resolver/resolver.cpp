#include <resolver.hpp>

std::string SCqResolver::Resolve()
{
    for(auto const & operation : root->children)
    {
        SCqNodeType operationType = operation->type;
        ScAddr agentResult;

        switch (operationType)
        {
        case SCqNodeType::Query:
            agentResult = LaunchQueryAgent(operation);
            break;

        case SCqNodeType::Mutation:
            agentResult = LaunchMutationAgent(operation);
            break;

        case SCqNodeType::Subscription:
            agentResult = LaunchSubscriptionAgent(operation);
            break;
        
        default:
            break;
        }
    }
}

ScAddr const &SCqResolver::LaunchQueryAgent(std::shared_ptr<SCqNode> operationRoot)
{
    std::vector<std::shared_ptr<SCqNode>> entities = operationRoot->children;

    for (auto const & entity: entities)
    {
        ScAddr const & agentArguments = m_context.GenerateStructure();

        PrepareRelationsStructure(agentArguments, entity->children);
    }
}

ScAddr const &SCqResolver::LaunchMutationAgent(std::shared_ptr<SCqNode> operationRoot)
{
    
}

ScAddr const &SCqResolver::LaunchSubscriptionAgent(std::shared_ptr<SCqNode> operationRoot)
{
    
}

void SCqResolver::PrepareRelationsStructure(ScAddr const &agentRelArgument, std::vector<std::shared_ptr<SCqNode>> const &relsNodes)
{
    for (auto const &relNode : relsNodes)
    {
        ScAddr const & rel = m_context.SearchElementBySystemIdentifier(relNode->value);

        if (relNode->children.size() > 1)
        {
            ScAddr const & nestedRelStruct = m_context.GenerateStructure();

            ScAddr const & arg = CheckArgumentValue(relNode);

            if(arg.IsValid())
            {
                GenerateRelationArgument(rel, arg);
            } 

        }
        else if (relNode->children.size() == 1)
        {
            if(relNode->children[0]->type == SCqNodeType::Argument)
            {
                
            }
            m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, agentRelArgument, m_context.SearchElementBySystemIdentifier(relNode->value));
        }
        else
        {

        }
    }
}

ScAddr const &SCqResolver::CheckArgumentValue(std::shared_ptr<SCqNode> argument)
{
    if(argument->children[0]->type == SCqNodeType::Argument)
    {
        return m_context.SearchElementBySystemIdentifier(argument->children[0]->value);
    }
}
