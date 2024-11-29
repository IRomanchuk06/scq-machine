#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>

enum class TokenType {
    Keyword = 1,
    Identifier = 2,
    Variable = 3,
    Directive = 4,
    OutputType = 5,
    CurlyBraceOpen = 6,
    CurlyBraceClose = 7,
    ParenOpen = 8,
    Exclamation = 9,
    ParenClose = 10,
    Colon = 11,
    Comma = 12,
    StringLiteral = 13,
    Number = 14,
    EndOfInput = 15,
    Ellipsis = 16
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, std::string const & value) : type(type), value(value) {}
};

class Tokenizer
{
public:
    Tokenizer(std::string const & _source):
        source(_source), position(0){};

    std::vector<Token> Tokenize();

private:
    std::string source;
    size_t position;

    Token ReadKeywordOrIdentifier();
    Token ReadNumber();
    Token ReadStringLiteral();
    Token ReadVariable();
    Token ReadDirective();
    Token ReadElipsis();

    static std::unordered_set<std::string> const keywords;
    static std::unordered_set<std::string> const outputTypes;
};