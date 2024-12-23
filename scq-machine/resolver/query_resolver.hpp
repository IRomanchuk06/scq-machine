#pragma once 

#include "resolver.hpp"
#include "resolver_constants.hpp"

class SCqQueryResolver : public SCqResolver
{
public:
    SCqQueryResolver(std::shared_ptr<SCqNode> const _queryRoot):
        SCqResolver(_queryRoot)
        {
            queryRoot = std::static_pointer_cast<SCqQuery>(this->root);
        };

    std::string Resolve() override;

protected:
    std::shared_ptr<SCqQuery> queryRoot;

    std::string GenerateJSON(ScAddr const& actionQuery) override;
    ScAddr CreateArgsStruct(std::shared_ptr<SCqNode> entity) override;
};