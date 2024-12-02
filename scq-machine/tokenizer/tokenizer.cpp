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
                tokens.emplace_back(SCqTokenType::CurlyBraceOpen, "{");
                position++;
                break;
            case '}':
                tokens.emplace_back(SCqTokenType::CurlyBraceClose, "}");
                position++;
                break;
            case '(':
                tokens.emplace_back(SCqTokenType::ParenOpen, "(");
                position++;
                break;
            case ')':
                tokens.emplace_back(SCqTokenType::ParenClose, ")");
                position++;
                break;
            case ':':
                tokens.emplace_back(SCqTokenType::Colon, ":");
                position++;
                break;
            case ',':
                tokens.emplace_back(SCqTokenType::Comma, ",");
                position++;
                break;
            case '!':
                tokens.emplace_back(SCqTokenType::Exclamation, "!");
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
            case '.':
                tokens.push_back(ReadElipsis());
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

    tokens.emplace_back(SCqTokenType::EndOfInput, "");
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
        return Token(SCqTokenType::Keyword, value);
    } 
    else if (outputTypes.find(value) != outputTypes.end())
    {
        return Token(SCqTokenType::OutputType, value);
    }

    return Token(SCqTokenType::Identifier, value);
}

Token Tokenizer::ReadNumber()
{
    size_t start = position;

    while (position < source.size() && isdigit(source[position])) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(SCqTokenType::Number, value);
}

Token Tokenizer::ReadStringLiteral()
{
    position++;
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
                    throw std::runtime_error(
                        "Unknown escape sequence in string literal: \\" + std::string(1, escaped));
            }
        } else if (current == '\"') {
            position++;
            return Token(SCqTokenType::StringLiteral, value);
        } else {
            value += current;
        }

        position++;
    }

    throw std::runtime_error("Unterminated string literal at position " + std::to_string(position));
}

Token Tokenizer::ReadVariable()
{
    position++;
    size_t start = position;

    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(SCqTokenType::Variable, value);
}

Token Tokenizer::ReadDirective() 
{
    position++; 
    size_t start = position;
    
    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }

    std::string value = source.substr(start, position - start);
    return Token(SCqTokenType::Directive, value);
}

Token Tokenizer::ReadElipsis()
{
    if (position + 2 < source.size() && source[position + 1] == '.' && source[position + 2] == '.') {
        position += 3;
        return Token(SCqTokenType::Ellipsis, "...");
    } else {
        throw std::runtime_error("Unknown character in input at position " + std::to_string(position));
    }
}
