#pragma once

#include "../scq_node.hpp"

#include <unordered_map>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_keynodes.hpp>

#include "../../resolver/keynodes.hpp"

class SCqResolverKeynodes;

struct SCqMutation : public SCqNode
{
    explicit SCqMutation(const SCqNodeType& nodeType, std::string const & scqMutationOperationName,std::string const & value = "");

    enum class SCqMutationType
    {
        MutationEntityClasses,
        MutationRelatedEntities
    };

    static const std::unordered_map<std::string, SCqMutationType> operationNametoType;
    static const std::unordered_map<SCqMutationType, std::string> operationTypeToName;

    static const std::unordered_map<SCqMutationType , ScKeynode> operationTypeToOperationActionClass;

    std::string GetOperationName() const;
    SCqMutationType GetOperationType () const;
    ScKeynode GetOperationActionClass() const;

    SCqMutationType operationType;
};