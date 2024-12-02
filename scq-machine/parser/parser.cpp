#include "parser.hpp"

#include "operation_parser.hpp"

std::shared_ptr<SCqNode> SCqParser::Parse()
{
    if (context.IsTokensEmpty())
    {
        throw std::runtime_error("Unexpected empty input");
    }

    auto root = std::make_shared<SCqNode>(SCqNodeType::Query);

    while (!context.IsEnd())
    {
        if (context.CurrentToken().type == SCqTokenType::Keyword)
        {
            std::string tokenValue = context.CurrentToken().value;
            
            std::transform(tokenValue.begin(), tokenValue.end(), tokenValue.begin(), ::tolower);

            if (tokenValue == "query" || tokenValue == "mutation" || tokenValue == "subscription")
            {
                SCqOperationParser operationParser(context);
                root->children.push_back(operationParser.Parse());
            }
            else if (tokenValue == "fragment")
            {
                
            }
        }

    }
    
    return root;
}