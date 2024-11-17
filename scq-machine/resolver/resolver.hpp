#pragma once

#include "parser.hpp"

class SCqResolver
{
public:
    SCqResolver(std::shared_ptr<SCqNode> const _root):
        root(_root){};

    std::string Resolve();

private:
    std::shared_ptr<SCqNode> const root;
    
};