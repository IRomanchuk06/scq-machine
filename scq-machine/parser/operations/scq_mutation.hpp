#pragma once

#include "../scq_node.hpp"

#include <unordered_map>

struct SCqMutation : public SCqNode
{
    explicit SCqMutation(const SCqNodeType& nodeType, std::string const & scqMutationOperationName,std::string const & value = "");


    enum class SCqMutationType
    {
        MutationEntityClasses,
        MutationRelatedEntities
    };

    static const std::unordered_map<std::string, SCqMutationType> operations;

    SCqMutationType operationType;
};