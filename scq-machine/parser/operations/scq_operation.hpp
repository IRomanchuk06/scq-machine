#pragma once

#include "../scq_node.hpp"

class SCqOperation: SCqNode
{
public: 
    SCqOperation(SCqNodeType const& _type, std::string const& _value = "")
        :SCqNode(_type, _value) {};

    void AddNested(std::shared_ptr<SCqNode> nested);

private:
    std::vector<std::shared_ptr<SCqNode>> nestedOperations;
};