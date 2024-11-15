#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>

enum class TokenType {
    Keyword,
    Identifier,
    Variable,
    Directive,
    OutputType,
    CurlyBraceOpen,
    CurlyBraceClose,
    ParenOpen,
    Exclamation,
    ParenClose,
    Colon,
    Comma,
    StringLiteral,
    Number,
    EndOfInput
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

    static std::unordered_set<std::string> const keywords;
    static std::unordered_set<std::string> const outputTypes;
};