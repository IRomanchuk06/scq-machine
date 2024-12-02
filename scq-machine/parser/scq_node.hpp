#pragma once

#include <vector>
#include <string>
#include <memory>

enum class SCqNodeType {
    Entity,
    Query,
    Mutation,
    Subscription,
    Fragment,
    Field,
    Argument,
    Identifier,
    StringLiteral,
    Number,
    Variable,
    Type
};

class SCqNode {
public:
    SCqNodeType type;
    std::string value;
    std::vector<std::shared_ptr<SCqNode>> children;

    SCqNode(SCqNodeType const& type, std::string const & value = "")
        : type(type), value(value) {}

};