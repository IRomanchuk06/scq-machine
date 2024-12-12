#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/keynodes.hpp"

#include "../../../../../scq-machine/parser/parser.hpp"
#include "../../../../../scq-machine/resolver/keynodes.hpp"


class QueryRelatedEntitiesAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const;

  ScResult DoProgram(ScAction & action);

  ScAddr const & FindNodesFromRelsStruct(ScAddr const & entity, ScAddr const & relsStruct, ScAddr const & resultStruct);
};
