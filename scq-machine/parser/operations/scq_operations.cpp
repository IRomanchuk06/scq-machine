#include "scq_query.hpp"
#include "scq_mutation.hpp"
#include "scq_subscription.hpp"

#include <iostream>

#include <sc-memory/sc_debug.hpp>

// Query

SCqQuery::SCqQuery(const SCqNodeType &nodeType, std::string const &scqQueryOperationName, std::string const &value):
    SCqNode(nodeType, value)
{
    auto it = SCqQuery::operationNameToType.find(scqQueryOperationName);
    if (it != SCqQuery::operationNameToType.end()) {
        operationType = it->second;
    } else {
        throw std::invalid_argument("Invalid operation name: " + scqQueryOperationName);
    }
}

std::string SCqQuery::GetOperationName() const
{
    auto it = operationTypeToName.find(this->operationType);
    if(it != operationTypeToName.end())
    {
        return it->second;
    }
        return ""; 
}

SCqQuery::SCqQueryType SCqQuery::GetOperationType() const
{
    return this->operationType;
}

ScKeynode SCqQuery::GetOperationActionClass() const
{
    std::unordered_map<SCqQuery::SCqQueryType, ScKeynode> const operationTypeToOperationActionClass = {
        {SCqQuery::SCqQueryType::QueryRelatedEntities, SCqResolverKeynodes::action_query_related_entities}
    }; // init if not 

    auto it = operationTypeToOperationActionClass.find(GetOperationType());
    if(it != operationTypeToOperationActionClass.end())
    {
        return it->second;
    }
    // SC_THROW_EXCEPTION
}

std::unordered_map<SCqQuery::SCqQueryType, std::string> const SCqQuery::operationTypeToName = {
    {SCqQuery::SCqQueryType::QueryEntityClasses, "QueryEntityClasses"},
    {SCqQuery::SCqQueryType::QueryRelatedEntities, "QueryRelatedEntities"}
};

std::unordered_map<std::string, SCqQuery::SCqQueryType> const SCqQuery::operationNameToType = {
    {"QueryEntityClasses", SCqQuery::SCqQueryType::QueryEntityClasses},
    {"QueryRelatedEntities", SCqQuery::SCqQueryType::QueryRelatedEntities}
};

// Mutation

SCqMutation::SCqMutation(const SCqNodeType &nodeType, std::string const &scqMutationOperationName, std::string const &value):
    SCqNode(nodeType, value)

{
    auto it = SCqMutation::operationNametoType.find(scqMutationOperationName);
    if (it != SCqMutation::operationNametoType.end()) {
        operationType = it->second;
    } else {
        throw std::invalid_argument("Invalid operation name: " + scqMutationOperationName);
    }
}

std::string SCqMutation::GetOperationName() const
{
    auto it = operationTypeToName.find(this->operationType);
    if(it != operationTypeToName.end())
    {
        return it->second;
    }
    return ""; 
}

SCqMutation::SCqMutationType SCqMutation::GetOperationType() const
{
    return this->operationType;
}

ScKeynode SCqMutation::GetOperationActionClass() const
{
    auto it = operationTypeToOperationActionClass.find(GetOperationType());
    if(it != operationTypeToOperationActionClass.end())
    {
        return it->second;
    }
    // throw
}

std::unordered_map<std::string, SCqMutation::SCqMutationType> const SCqMutation::operationNametoType = {
    {"MutationEntityClasses", SCqMutation::SCqMutationType::MutationEntityClasses},
    {"MutationRelatedEntities", SCqMutation::SCqMutationType::MutationRelatedEntities}
};

std::unordered_map<SCqMutation::SCqMutationType, std::string> const SCqMutation::operationTypeToName = {
    {SCqMutation::SCqMutationType::MutationEntityClasses, "MutationEntityClasses"},
    {SCqMutation::SCqMutationType::MutationRelatedEntities, "MutationRelatedEntities"}
};

std::unordered_map<SCqMutation::SCqMutationType, ScKeynode> const SCqMutation::operationTypeToOperationActionClass = {
    {SCqMutation::SCqMutationType::MutationRelatedEntities, SCqResolverKeynodes::action_mutation_related_entities}
};

// Subcription

SCqSubscription::SCqSubscription(const SCqNodeType &nodeType, std::string const &scqSubscriptionOperationName, std::string const &value):
    SCqNode(nodeType, value)
{
    auto it = SCqSubscription::operations.find(scqSubscriptionOperationName);
    if (it != SCqSubscription::operations.end()) {
        operationType = it->second;
    } else {
        throw std::invalid_argument("Invalid operation name: " + scqSubscriptionOperationName);
    }
}

std::unordered_map<std::string, SCqSubscription::SCqSubscriptionType> const SCqSubscription::operations = {
    {"SubscriptionTrackElement" , SCqSubscription::SCqSubscriptionType::SubscriptionTrackElement}
};