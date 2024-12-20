#include "parser.hpp"

#include "fragment_parser.hpp"
#include "operations/operation_parser.hpp"

std::shared_ptr<SCqNode> SCqParser::Parse()
{
    if (context.IsTokensEmpty())
    {
        throw std::runtime_error("Unexpected empty input");
    }

    auto root = std::make_shared<SCqNode>(SCqNodeType::Query);

    while (!context.IsEnd())
    {
        auto token = context.CurrentToken();

        if (token.type == SCqTokenType::CurlyBraceOpen)
        {
            context.Advance();
        }
        else if (token.type == SCqTokenType::Keyword)
        {
            if (token.value == "fragment")
            {
                SCqFragmentParser fragmentParser(context);

                auto fragment = fragmentParser.Parse();
                std::string const & fragmentName = fragment->value;

                context.AddFragment(fragmentName, fragment);

                context.Advance();
            }
            else if (GetNodeTypeFromOperationName(token.value) == SCqNodeType::Query || GetNodeTypeFromOperationName(token.value) == SCqNodeType::Mutation || GetNodeTypeFromOperationName(token.value) == SCqNodeType::Subscription) 
            {
                SCqOperationParser operationParser(context);

                root->children.push_back(operationParser.Parse());

                context.Advance();
            }
            else
            {
                // throw
            }
        }
        else
        {
            // throw
        }
    }
    
    return root;
}

SCqNodeType SCqParser::GetNodeTypeFromOperationName(std::string const &operationName)
{
    if(SCqQuery::operationNameToType.find(operationName) != SCqQuery::operationNameToType.end())
    {
        return SCqNodeType::Query;
    }
    else if (SCqMutation::operationNametoType.find(operationName) != SCqMutation::operationNametoType.end())
    {
        return SCqNodeType::Mutation;
    }
    else if (SCqSubscription::operations.find(operationName) != SCqSubscription::operations.end())
    {
        return SCqNodeType::Subscription;
    }

    return SCqNodeType::Field;
}
