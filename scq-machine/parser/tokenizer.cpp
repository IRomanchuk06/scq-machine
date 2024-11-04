#include "tokenizer.hpp"


std::set<std::string> const Tokenizer::keywords = {
    "query", "mutation", "subscription", "fragment", "on", "true", "false", "null"
};

std::set<std::string> const Tokenizer::outputTypes = {
    "String", "Int", "Float", "Boolean", "ID"
};

std::vector<Token> Tokenizer::Tokenize()
{
    std::vector<Token> tokens;
    while (position < source.size()) {
        char current = source[position];

        if (isspace(current)) {
            position++;
            continue;
        } else if (current == '{') {
            tokens.emplace_back(TokenType::CurlyBraceOpen, "{");
            position++;
        } else if (current == '}') {
            tokens.emplace_back(TokenType::CurlyBraceClose, "}");
            position++;
        } else if (current == '(') {
            tokens.emplace_back(TokenType::ParenOpen, "(");
            position++;
        } else if (current == ')') {
            tokens.emplace_back(TokenType::ParenClose, ")");
            position++;
        } else if (current == ':') {
            tokens.emplace_back(TokenType::Colon, ":");
            position++;
        } else if (current == ',') {
            tokens.emplace_back(TokenType::Comma, ",");
            position++;
        } else if (current == '!') {
            tokens.emplace_back(TokenType::Exclamation, "!");
            position++;
        } else if (current == '$') {
            tokens.push_back(ReadVariable());
        } else if (current == '@') {
            tokens.push_back(ReadDirective()); 
        } else if (isalpha(current)) {
            tokens.push_back(ReadKeywordOrIdentifier());
        } else if (isdigit(current) || current == '-') {
            tokens.push_back(ReadNumber());
        } else if (current == '\"') {
            tokens.push_back(ReadStringLiteral());
        } else {
            throw std::runtime_error("Unknown character in input");
        }
    }

    tokens.emplace_back(TokenType::EndOfInput, "");
    return tokens;
}

Token Tokenizer::ReadKeywordOrIdentifier()
{
    size_t start = position;

    while (position < source.size() && (isalnum(source[position]) || source[position] == '_'))
    {
        position++;
    }

    std::string value = source.substr(start, position - start);

    if (keywords.find(value) != keywords.end())
    {
        return Token(TokenType::Keyword, value);
    } 
    else if (outputTypes.find(value) != outputTypes.end())
    {
        return Token(TokenType::OutputType, value);
    }

    return Token(TokenType::Identifier, value);
}

Token Tokenizer::ReadNumber()
{
    size_t start = position;

    while (position < source.size() && isdigit(source[position])) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(TokenType::Number, value);
}

Token Tokenizer::ReadStringLiteral()
{
    position++;
    size_t start = position;

    while (position < source.size() && source[position] != '\"') {
        position++;
    }

    std::string value = source.substr(start, position - start);
    position++;  
    return Token(TokenType::StringLiteral, value);
}

Token Tokenizer::ReadVariable()
{
    position++;
    size_t start = position;

    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(TokenType::Variable, value);
}

Token Tokenizer::ReadDirective() 
{
    position++; 
    size_t start = position;
    
    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(TokenType::Directive, value);
}
