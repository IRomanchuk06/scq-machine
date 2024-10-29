#pragma once

#include "../scq-model/query.hpp"

class SCqQuery;

class SCqParser
{
public:
    static SCqQuery Parse(std::string const & queryStr);

private:
};