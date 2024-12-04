#include "parser.hpp"

#include "fragment_parser.hpp"
#include "operation_parser.hpp"

std::unordered_set<std::string> const SCqParser::queryOperations = {
    "QueryRelatedEntities", "QueryEntityClasses"
};

std::unordered_set<std::string> const SCqParser::mutationOperations = {
    "MutationEntityClasses", "MutationRelatedEntities"
};    

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
            else if (queryOperations.find(token.value) != queryOperations.end() || mutationOperations.find(token.value) != mutationOperations.end())
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