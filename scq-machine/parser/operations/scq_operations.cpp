#include "scq_query.hpp"
#include "scq_mutation.hpp"
#include "scq_subscription.hpp"

#include <iostream>

// Query

SCqQuery::SCqQuery(const SCqNodeType &nodeType, std::string const &scqQueryOperationName, std::string const &value):
    SCqNode(nodeType, value)
{
    auto it = SCqQuery::operations.find(scqQueryOperationName);
    if (it != SCqQuery::operations.end()) {
        operationType = it->second;
    } else {
        throw std::invalid_argument("Invalid operation name: " + scqQueryOperationName);
    }
}

std::unordered_map<std::string, SCqQuery::SCqQueryType> const SCqQuery::operations = {
    {"QueryEntityClasses", SCqQuery::SCqQueryType::QueryEntityClasses},
    {"QueryRelatedEntities", SCqQuery::SCqQueryType::QueryRelatedEntities}
};

// Mutation

SCqMutation::SCqMutation(const SCqNodeType &nodeType, std::string const &scqMutationOperationName, std::string const &value):
    SCqNode(nodeType, value)

{
    auto it = SCqMutation::operations.find(scqMutationOperationName);
    if (it != SCqMutation::operations.end()) {
        operationType = it->second;
    } else {
        throw std::invalid_argument("Invalid operation name: " + scqMutationOperationName);
    }
}

std::unordered_map<std::string, SCqMutation::SCqMutationType> const SCqMutation::operations = {
    {"MutationEntityClasses", SCqMutation::SCqMutationType::MutationEntityClasses},
    {"MutationRelatedEntities", SCqMutation::SCqMutationType::MutationRelatedEntities}
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