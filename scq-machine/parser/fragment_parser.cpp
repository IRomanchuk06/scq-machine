#include "fragment_parser.hpp"

std::shared_ptr<SCqNode> SCqFragmentParser::Parse()
{
    context.ExpectToken(SCqTokenType::Keyword);
    context.Advance();

    context.ExpectToken(SCqTokenType::Identifier);
    auto fragmentRoot = std::make_shared<SCqNode>(SCqNodeType::Fragment, context.CurrentToken().value);
    context.Advance();

    context.ExpectToken(SCqTokenType::CurlyBraceOpen);
    context.Advance();

    while(context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
    {
        fragmentRoot->children.push_back(ParseField());
    } 

    context.Advance();

    return fragmentRoot;
}

std::shared_ptr<SCqNode> SCqFragmentParser::ParseField()
{
    if(context.CurrentToken().type == SCqTokenType::Ellipsis)
    {
        context.Advance();
        context.ExpectToken(SCqTokenType::Identifier);

        std::string fragmentName = context.CurrentToken().value;
        context.Advance();

        return context.GetFragment(fragmentName);
    }

    context.ExpectToken(SCqTokenType::Identifier);
    std::string fieldName = context.CurrentToken().value;
    context.Advance();

    auto fieldRoot = std::make_shared<SCqNode>(SCqNodeType::Field, fieldName);

    // check for nested
    if(context.CurrentToken().type == SCqTokenType::CurlyBraceOpen)
    {
        context.Advance();
        while (context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
        {
            fieldRoot->children.push_back(Parse());
        }
        context.Advance();
    }

    return fieldRoot;
}
