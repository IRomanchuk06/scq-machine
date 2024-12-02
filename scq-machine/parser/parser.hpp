#pragma once

#include "parser_context.hpp"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

class ScqParserContext;

class SCqParser {
protected:
    SCqParserContext& context;

public:
    explicit SCqParser(SCqParserContext & _context) : context(_context) {}

    virtual std::shared_ptr<SCqNode> Parse();
};

