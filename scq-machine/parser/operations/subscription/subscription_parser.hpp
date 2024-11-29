#pragma once

#include "../../parser.hpp"
#include "../../scq_node.hpp"

#include <memory>

class SCqSubscriptionParser : public SCqParser
{
public:
    explicit SCqSubscriptionParser(SCqParserContext& _parserContext)
        :SCqParser(_parserContext) {};

    std::shared_ptr<SCqNode> Parse() override;
};