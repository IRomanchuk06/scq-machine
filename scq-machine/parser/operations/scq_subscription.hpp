#pragma once

#include "../scq_node.hpp"

#include <unordered_map>

struct SCqSubscription : public SCqNode
{
    explicit SCqSubscription(const SCqNodeType& nodeType, std::string const & scqSubscriptionOperationName,std::string const & value = "");


    enum class SCqSubscriptionType
    {
        SubscriptionTrackElement
    };

    static std::unordered_map<std::string, SCqSubscriptionType> const operations;

    SCqSubscriptionType operationType;
};
