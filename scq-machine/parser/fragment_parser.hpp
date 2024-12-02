#pragma once

#include "parser.hpp"

class SCqFragmentParser : public SCqParser
{
public:
    explicit SCqFragmentParser(SCqParserContext& _context) : SCqParser(_context){}

    std::shared_ptr<SCqNode> Parse() override;

private:
    std::shared_ptr<SCqNode> ParseField();
};