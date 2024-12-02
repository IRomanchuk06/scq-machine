#pragma once

#include "scq_node.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <unordered_map>
#include <iostream>

class SCqParserContext
{
public:
    explicit SCqParserContext(const std::vector<Token>& tokens)
        : position(0), tokens(tokens) {}

    Token const& CurrentToken() const;
    void Advance();
    bool ExpectToken(SCqTokenType const& expectedToken) const;

    void AddFragment(const std::string& fragmentName, std::shared_ptr<SCqNode> fragment);
    std::shared_ptr<SCqNode> GetFragment(const std::string& fragmentName) const;

    bool IsTokensEmpty() const;
    bool IsEnd() const;

private:
    size_t position;
    const std::vector<Token>& tokens;
    std::unordered_map<std::string, std::shared_ptr<SCqNode>> fragments;
};