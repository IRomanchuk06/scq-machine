#pragma once

#include "scq_node.hpp"
#include "parser_context.hpp"
#include "parser.hpp"

#include <algorithm>
#include <cctype>

class SCqOperationParser : public SCqParser
{
public:
    explicit SCqOperationParser(SCqParserContext& _context) : SCqParser(_context){}

    std::shared_ptr<SCqNode> Parse() override;

private:
    std::unordered_map<std::string, std::vector<std::string>> arguments;

    SCqNodeType GetOperationSCqTypeFromOperationName(std::string const& operationName);
    std::shared_ptr<SCqNode> ParseEntity();
    std::shared_ptr<SCqNode> ParseField();
    
    void ParseFieldArguments();
};