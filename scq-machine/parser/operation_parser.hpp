#pragma once

#include "scq_node.hpp"
#include "parser_context.hpp"

class SCqOperationParser
{
public:
    std::shared_ptr<SCqNode> Parse(SCqParserContext & context);

private:
    std::unordered_map<std::string, std::string> arguments;

    SCqNodeType GetOperationSCqTypeFromOperationName(std::string const & operationStr);
    std::shared_ptr<SCqNode> ParseEntity(SCqParserContext & context);
    std::shared_ptr<SCqNode> ParseField(SCqParserContext & context);
    
    void ParseFieldArguments(SCqParserContext & context);
};