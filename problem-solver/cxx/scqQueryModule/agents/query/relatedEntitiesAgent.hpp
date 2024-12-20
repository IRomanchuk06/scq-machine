#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/scq_agents_keynodes.hpp"


class QueryRelatedEntitiesAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const;

  ScResult DoProgram(ScAction & action);

  ScAddr FindNodesFromRelsStruct(ScAddr const & entity, ScAddr const & relsStruct, ScAddr const & resultStruct);
};
