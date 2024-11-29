#pragma once

#include "../../parser.hpp"
#include "../../scq_node.hpp"

#include <memory>

class SCqQueryParser : public SCqParser
{
public:
    explicit SCqQueryParser(SCqParserContext& _parserContext) : SCqParser(_parserContext) {}

    std::shared_ptr<SCqNode> Parse() override;
};