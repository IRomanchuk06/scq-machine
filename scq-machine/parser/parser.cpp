#include "parser.hpp"

std::shared_ptr<SCqNode> SCqParser::Parse()
{
    if (parseContext.IsTokensEmpty())
    {
        throw std::runtime_error("Unexpected empty input");
    }

    auto root = std::make_shared<SCqNode>(SCqNodeType::Query);

    while (!parseContext.IsEnd())
    {
        if (parseContext.CurrentToken().type == SCqTokenType::Keyword)
        {
            if (parseContext.CurrentToken().value == "query")
            {

            }
            else if (parseContext.CurrentToken().value == "mutation")
            {
                
            }
            else if (parseContext.CurrentToken().value == "subscription")
            {
                
            }
            else if (parseContext.CurrentToken().value == "fragment")
            {

            }
        }
    }
    
    return root;
}