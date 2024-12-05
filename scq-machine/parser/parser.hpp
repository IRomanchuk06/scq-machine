#pragma once

#include "parser_context.hpp"

#include "operations/scq_query.hpp"
#include "operations/scq_mutation.hpp"
#include "operations/scq_subscription.hpp"

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

    SCqNodeType GetNodeTypeFromOperationName(std::string const & operationName);
};

