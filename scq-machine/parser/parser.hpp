#pragma once

#include "../tokenizer/tokenizer.hpp"
#include "scq_node.hpp"

#include <iostream>
#include <unordered_map>

struct SCqParserContext
{
    size_t position = 0; 
    const std::vector<Token>& tokens;
    
    std::unordered_map<std::string, std::shared_ptr<SCqNode>> fragments;

    explicit SCqParserContext(const std::vector<Token>& tokens) : tokens(tokens) {}


    Token const& CurrentToken() const;
    void Advance();
    bool ExpectToken(TokenType const& expectedToken) const;

    void AddFragment(std::string const & fragmentName, std::shared_ptr<SCqNode> fragment);
    
    bool const IsTokensEmpty() const;
    bool const IsEnd() const;
};

class SCqParser {
protected:
    SCqParserContext& parseContext;

public:
    explicit SCqParser(SCqParserContext & _parseContext) : parseContext(_parseContext) {}
    virtual ~SCqParser() = default;

    virtual std::shared_ptr<SCqNode> Parse();
};

#include "operations/query/query_parser.hpp"
#include "operations/mutation/mutation_parser.hpp"
#include "operations/subscription/subscription_parser.hpp"