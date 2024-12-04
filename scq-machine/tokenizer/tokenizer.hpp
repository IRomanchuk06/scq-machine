#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>

enum class SCqTokenType {
    Keyword,
    Identifier,
    CurlyBraceOpen,
    CurlyBraceClose,
    ParenOpen,
    ParenClose,
    SquareBraceOpen,
    SquareBraceClose,
    Colon,
    Comma,
    StringLiteral,
    Ellipsis,
    EndOfInput
};

struct Token {
    SCqTokenType type;
    std::string value;

    Token(SCqTokenType type, std::string const & value) : type(type), value(value) {}
};

class SCqTokenizer
{
public:
    SCqTokenizer(std::string const & _source):
        source(_source), position(0){};

    std::vector<Token> Tokenize();

private:
    std::string source;
    size_t position;

    Token ReadKeywordOrIdentifier();
    Token ReadStringLiteral();
    Token ReadElipsis();

    static std::unordered_set<std::string> const keywords;
};