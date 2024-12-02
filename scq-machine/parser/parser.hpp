#pragma once

#include "parser_context.hpp"
#include "operation_parser.hpp"

#include <iostream>
#include <unordered_map>

class SCqParser {
protected:
    SCqParserContext& parseContext;

public:
    explicit SCqParser(SCqParserContext & _parseContext) : parseContext(_parseContext) {}
    virtual ~SCqParser() = default;

    virtual std::shared_ptr<SCqNode> Parse();
};