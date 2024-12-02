#pragma once

#include "scq_node.hpp"
#include "../tokenizer/tokenizer.hpp"

#include <unordered_map>
#include <iostream>

struct SCqParserContext
{
    size_t position = 0; 
    const std::vector<Token>& tokens;
    
    std::unordered_map<std::string, std::shared_ptr<SCqNode>> fragments;

    explicit SCqParserContext(const std::vector<Token>& tokens) : tokens(tokens) {}


    Token const& CurrentToken() const;
    void Advance();
    bool ExpectToken(SCqTokenType const& expectedToken) const;

    void AddFragment(std::string const & fragmentName, std::shared_ptr<SCqNode> fragment);
    
    bool const IsTokensEmpty() const;
    bool const IsEnd() const;
};
