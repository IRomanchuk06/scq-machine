#include "query_parser.hpp"

std::shared_ptr<SCqNode> SCqQueryParser::Parse()
{
    parseContext.ExpectToken(TokenType::Keyword);
    auto queryRoot = std::make_shared<SCqNode>(parseContext.CurrentToken().type);

    parseContext.Advance();

    if(parseContext.CurrentToken().type == TokenType::Identifier)
    {
        queryRoot->value=parseContext.CurrentToken().value;
        parseContext.Advance();
    }

    parseContext.ExpectToken(TokenType::CurlyBraceOpen);
    parseContext.Advance();

    parseContext.ExpectToken(TokenType::Identifier);
    

    return queryRoot;
}