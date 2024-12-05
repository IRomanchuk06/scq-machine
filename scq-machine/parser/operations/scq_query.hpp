#pragma once

#include "../scq_node.hpp"

#include <unordered_map>

struct SCqQuery : public SCqNode
{
    explicit SCqQuery(const SCqNodeType& nodeType, std::string const & scqQueryOperationName,std::string const & value = "");

    enum class SCqQueryType
    {
        QueryEntityClasses,
        QueryRelatedEntities
    };

    static const std::unordered_map<std::string, SCqQueryType> operations;

    SCqQueryType operationType;
};


