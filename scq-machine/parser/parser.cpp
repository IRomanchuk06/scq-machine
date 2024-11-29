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
        if (parseContext.CurrentToken().type == TokenType::Keyword)
        {
            if (parseContext.CurrentToken().value == "query")
            {
                auto queryParser = SCqQueryParser(parseContext);
                std::shared_ptr<SCqNode> operation = queryParser.Parse();
            }
            else if (parseContext.CurrentToken().value == "mutation")
            {
                auto mutationParser = SCqMutationParser(parseContext);
                std::shared_ptr<SCqNode> operation = mutationParser.Parse();
            }
            else if (parseContext.CurrentToken().value == "subscription")
            {
                auto subscriptionParser = SCqSubscriptionParser(parseContext);
                std::shared_ptr<SCqNode> operation = subscriptionParser.Parse();
            }
            else if (parseContext.CurrentToken().value == "fragment")
            {

            }
        }
    }
    

    /*
    while (position < tokens.size()) {
        if (CurrentToken().type == TokenType::Keyword) {
            if (CurrentToken().value == "query") {
                root->children.push_back(ParseQuery());
            } else if (CurrentToken().value == "mutation") {
                root->children.push_back(ParseMutation());
            } else if (CurrentToken().value == "subscription") {
                root->children.push_back(ParseSubscription());
            } else if (CurrentToken().value == "fragment") {
                auto const fragment = ParseFragment();
                fragments.insert({fragment->value, fragment});
            } else {
                throw std::runtime_error("Unexpected keyword: " + CurrentToken().value);
            } 
        } else if (CurrentToken().type == TokenType::Identifier) {
            root->children.push_back(ParseField());
        } else {
            throw std::runtime_error("Unexpected token: " + CurrentToken().value);
        }
    }*/

    return root;
}

Token const &SCqParserContext::CurrentToken() const
{
    if (position >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[position];
}

void SCqParserContext::Advance()
{
    if (position < tokens.size()) {
        position++;
    }
}

bool SCqParserContext::ExpectToken(TokenType const &expectedTokenType) const
{
    Token const& currentToken = CurrentToken();

    if (currentToken.type != expectedTokenType) {
        std::cerr << "Error: Expected token type '" 
                  << static_cast<int>(expectedTokenType) 
                  << "', but got '" 
                  << static_cast<int>(currentToken.type) 
                  << "'." << std::endl;
        return false;
    }

    return true;
}
void SCqParserContext::AddFragment(std::string const &fragmentName, std::shared_ptr<SCqNode> fragment)
{
    fragments[fragmentName] = fragment;
}

bool const SCqParserContext::IsTokensEmpty() const
{
    return tokens.empty();
}

bool const SCqParserContext::IsEnd() const
{
    return position > tokens.size();
}
