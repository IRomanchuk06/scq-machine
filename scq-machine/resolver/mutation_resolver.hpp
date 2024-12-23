#pragma once 

#include "resolver.hpp"
#include "resolver_constants.hpp"

class SCqMutationResolver : public SCqResolver
{
public:
    SCqMutationResolver(std::shared_ptr<SCqNode> const _mutationRoot):
        SCqResolver(_mutationRoot)
        {
            mutationRoot = std::static_pointer_cast<SCqMutation>(this->root);
        };

    std::string Resolve() override;

protected:
    std::shared_ptr<SCqMutation> mutationRoot;

    std::string GenerateJSON(ScAddr const& actionMutation) override;
    ScAddr CreateArgsStruct(std::shared_ptr<SCqNode> entity) override;
};