#include <parser.hpp>


std::shared_ptr<SCqNode> SCqParser::Parse()
{  
    if (tokens.empty()) {
        throw std::runtime_error("Unexpected empty input");
    }

    auto root = std::make_shared<SCqNode>(SCqNodeType::Query);

    while (position < tokens.size()) {
        if (CurrentToken().type == TokenType::Keyword) {
            if (CurrentToken().value == "query") {
                root->children.push_back(ParseQuery());
            } else if (CurrentToken().value == "mutation") {
                root->children.push_back(ParseMutation());
            } else if (CurrentToken().value == "subscription") {
                root->children.push_back(ParseSubscription());
            } else {
                throw std::runtime_error("Unexpected keyword: " + CurrentToken().value);
            }
        } else if (CurrentToken().type == TokenType::Identifier) {
            root->children.push_back(ParseField());
        } else {
            throw std::runtime_error("Unexpected token: " + CurrentToken().value);
        }
    }

    return root;
}

std::shared_ptr<SCqNode> SCqParser::ParseQuery()
{
    Expect(TokenType::Keyword);
    auto queryNode = std::make_shared<SCqNode>(SCqNodeType::Query, "");
    Advance();

    if (CurrentToken().type == TokenType::Identifier) {
        queryNode->value = CurrentToken().value;
        Advance();
    }

    if (CurrentToken().type == TokenType::ParenOpen) {
        Expect(TokenType::ParenOpen);
        Advance();

        auto argumentsList = std::make_shared<SCqNode>(SCqNodeType::Argument, "");

        while (CurrentToken().type == TokenType::Identifier) {
            argumentsList->children.push_back(ParseArgument());
        }

        Expect(TokenType::ParenClose);
        Advance();

        queryNode->children.push_back(argumentsList);
    }

    Expect(TokenType::CurlyBraceOpen);
    Advance();

    while (CurrentToken().type == TokenType::Identifier) {
        queryNode->children.push_back(ParseField());
    }

    Expect(TokenType::CurlyBraceClose);
    Advance();

    return queryNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseMutation()
{
    Expect(TokenType::Keyword);
    auto mutationNode = std::make_shared<SCqNode>(SCqNodeType::Mutation, "");
    Advance();

    if (CurrentToken().type == TokenType::Identifier) {
        mutationNode->value = CurrentToken().value;
        Advance();
    }

    Expect(TokenType::CurlyBraceOpen);
    Advance();

    while (CurrentToken().type == TokenType::Identifier) {
        mutationNode->children.push_back(ParseField());
    }

    Expect(TokenType::CurlyBraceClose);
    Advance();

    return mutationNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseSubscription()
{
    Expect(TokenType::Keyword);
    auto subscriptionNode = std::make_shared<SCqNode>(SCqNodeType::Subscription, "");
    Advance();

    if (CurrentToken().type == TokenType::Identifier) {
        subscriptionNode->value = CurrentToken().value;
        Advance();
    }

    Expect(TokenType::CurlyBraceOpen);
    Advance();

    while (CurrentToken().type == TokenType::Identifier) {
        subscriptionNode->children.push_back(ParseField());
    }

    Expect(TokenType::CurlyBraceClose);
    Advance();

    return subscriptionNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseField()
{
    auto fieldNode = std::make_shared<SCqNode>(SCqNodeType::Field, CurrentToken().value);
    Advance();

    if (CurrentToken().type == TokenType::ParenOpen) {
        fieldNode->children.push_back(ParseArgument());
    }

    return fieldNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseArgument()
{
    auto argumentNode = std::make_shared<SCqNode>(SCqNodeType::Argument);

    argumentNode->children.push_back(ParseIdentifier());
    Expect(TokenType::Colon);
    Advance();

    if (CurrentToken().type == TokenType::Number)
    {
        argumentNode->children.push_back(ParseNumber());
    }
    else if (CurrentToken().type == TokenType::StringLiteral)
    {
        argumentNode->children.push_back(ParseStringLiteral());
    }

    if (CurrentToken().type == TokenType::Comma) Advance();

    return argumentNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseIdentifier()
{
    auto identifierNode = std::make_shared<SCqNode>(SCqNodeType::Identifier, CurrentToken().value);
    Advance();
    return identifierNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseStringLiteral()
{
    auto stringNode = std::make_shared<SCqNode>(SCqNodeType::StringLiteral, CurrentToken().value);
    Advance();
    return stringNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseNumber()
{
    auto numberNode = std::make_shared<SCqNode>(SCqNodeType::Number, CurrentToken().value);
    Advance();
    return numberNode;
}

std::shared_ptr<SCqNode> SCqParser::ParseVariable() {
    auto variableNode = std::make_shared<SCqNode>(SCqNodeType::Variable, CurrentToken().value);
    Advance();
    return variableNode;
}

Token SCqParser::CurrentToken() const
{
    if (position >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[position];
}

void SCqParser::Expect(TokenType type)
{
    if (CurrentToken().type != type) {
        throw std::runtime_error("Expected token type " + std::to_string(static_cast<int>(type)) +
                                 ", but got " + std::to_string(static_cast<int>(CurrentToken().type)));
    }
}

void SCqParser::Advance()
{
    if (position < tokens.size()) {
        position++;
    }
}
