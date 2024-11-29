#pragma once

#include "../../parser.hpp"
#include "../../scq_node.hpp"

#include <memory>

class SCqMutationParser : public SCqParser {
public:
    explicit SCqMutationParser(SCqParserContext& _parserContext) : SCqParser(_parserContext) {}

    std::shared_ptr<SCqNode> Parse() override;
};