#pragma once

#include "../scq-model/query.hpp"

class SCqQuery;

class SCqParser
{
public:
    virtual SCqQuery Parse(std::string const & queryStr);

private:
};