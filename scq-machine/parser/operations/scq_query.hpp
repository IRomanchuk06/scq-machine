#pragma once

#include "../scq_node.hpp"

#include <unordered_map>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_keynodes.hpp>

#include "../../resolver/keynodes.hpp"

struct SCqQuery : public SCqNode
{
    explicit SCqQuery(const SCqNodeType& nodeType, std::string const & scqQueryOperationName,std::string const & value = "");

    enum class SCqQueryType
    {
        QueryEntityClasses,
        QueryRelatedEntities
    };

    static const std::unordered_map<std::string, SCqQueryType> operationNameToType;
    static const std::unordered_map<SCqQueryType, std::string> operationTypeToName;

    static const std::unordered_map<SCqQueryType , ScKeynode> operationTypeToOperationActionClass;

    std::string GetOperationName() const;
    SCqQueryType GetOperationType () const;

    ScKeynode GetOperationActionClass() const;
    
    SCqQueryType operationType;
};


