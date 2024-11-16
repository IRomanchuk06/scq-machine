#pragma once

#include "query.hpp"
#include "tokenizer.hpp"

class SCqQuery;

enum class SCqNodeType {
    Query = 0,
    Mutation = 1,
    Subscription = 2,
    Variable = 3,
    Field = 4,
    Argument = 5,
    StringLiteral = 6,
    Identifier = 7,
    Directive = 8,
    Number = 9
};

struct SCqNode {
    SCqNodeType type;
    std::string value;
    std::vector<std::shared_ptr<SCqNode>> children;

    SCqNode(SCqNodeType type, const std::string& value = "") : type(type), value(value) {}
};

class SCqParser
{
public:
    SCqParser(const std::vector<Token>& tokens): 
        tokens(tokens), position(0) {};

    std::shared_ptr<SCqNode> Parse();

private:
    const std::vector<Token>& tokens;
    size_t position;

    std::shared_ptr<SCqNode> ParseQuery();
    std::shared_ptr<SCqNode> ParseMutation();
    std::shared_ptr<SCqNode> ParseSubscription();
    std::shared_ptr<SCqNode> ParseField();
    std::shared_ptr<SCqNode> ParseArgument();
    std::shared_ptr<SCqNode> ParseVariable();
    std::shared_ptr<SCqNode> ParseStringLiteral();
    std::shared_ptr<SCqNode> ParseNumber();
    std::shared_ptr<SCqNode> ParseIdentifier();

    Token CurrentToken() const;

    void Expect(TokenType type);
    void Advance();
};