#include "tokenizer.hpp"


std::unordered_set<std::string> const Tokenizer::keywords = {
    "query", "mutation", "subscription", "fragment", "on", "true", "false", "null"
};

std::unordered_set<std::string> const Tokenizer::outputTypes = {
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
        }

        switch (current) {
            case '{':
                tokens.emplace_back(TokenType::CurlyBraceOpen, "{");
                position++;
                break;
            case '}':
                tokens.emplace_back(TokenType::CurlyBraceClose, "}");
                position++;
                break;
            case '(':
                tokens.emplace_back(TokenType::ParenOpen, "(");
                position++;
                break;
            case ')':
                tokens.emplace_back(TokenType::ParenClose, ")");
                position++;
                break;
            case ':':
                tokens.emplace_back(TokenType::Colon, ":");
                position++;
                break;
            case ',':
                tokens.emplace_back(TokenType::Comma, ",");
                position++;
                break;
            case '!':
                tokens.emplace_back(TokenType::Exclamation, "!");
                position++;
                break;
            case '$':
                tokens.push_back(ReadVariable());
                break;
            case '@':
                tokens.push_back(ReadDirective());
                break;
            case '\"':
                tokens.push_back(ReadStringLiteral());
                break;
            default:
                if (isalpha(current)) {
                    tokens.push_back(ReadKeywordOrIdentifier());
                } else if (isdigit(current) || current == '-') {
                    tokens.push_back(ReadNumber());
                } else {
                    throw std::runtime_error("Unknown character in input at position " + std::to_string(position));
                }
                break;
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
    std::string value;

    while (position < source.size()) {
        char current = source[position];

        if (current == '\\') {
            position++;
            if (position >= source.size()) {
                throw std::runtime_error("Unterminated escape sequence in string literal");
            }
            char escaped = source[position];
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '\\': value += '\\'; break;
                case '\"': value += '\"'; break;
                case 'r': value += '\r'; break;
                case 'b': value += '\b'; break;
                case 'f': value += '\f'; break;
                default:
                    throw std::runtime_error("Unknown escape sequence in string literal: \\" + std::string(1, escaped));
            }
        } else if (current == '\"') {
            position++;
            return Token(TokenType::StringLiteral, value);
        } else {
            value += current;
        }

        position++;
    }

    throw std::runtime_error("Unterminated string literal");
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
