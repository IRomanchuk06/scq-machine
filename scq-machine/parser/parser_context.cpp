#include "parser_context.hpp"

Token const &SCqParserContext::CurrentToken() const
{
    if (position >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[position];
}

void SCqParserContext::Advance()
{
    if (position < tokens.size()) {
        position++;
    }
}

bool SCqParserContext::ExpectToken(SCqTokenType const &expectedTokenType) const
{
    Token const& currentToken = CurrentToken();

    if (currentToken.type != expectedTokenType) {
        std::cerr << "Error: Expected token type '" 
                  << static_cast<int>(expectedTokenType) 
                  << "', but got '" 
                  << static_cast<int>(currentToken.type) 
                  << "'." << std::endl;
        return false;
    }

    return true;
}
void SCqParserContext::AddFragment(std::string const &fragmentName, std::shared_ptr<SCqNode> fragment)
{
    fragments[fragmentName] = fragment;
}

std::shared_ptr<SCqNode> SCqParserContext::GetFragment(const std::string& fragmentName) const {
    auto it = fragments.find(fragmentName);
    if (it != fragments.end()) {
        return it->second;
    }
    return nullptr; 
}

bool SCqParserContext::IsTokensEmpty() const
{
    return tokens.empty();
}

bool SCqParserContext::IsEnd() const
{
    return position+1 >= tokens.size();
}
