#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/scq_agents_keynodes.hpp"

class MutationRelatedEntitiesAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const;
  
  ScResult DoProgram(ScAction & action);

  ScAddr BuildRelsForEntity(ScAddr const & entity, ScAddr const & relsStruct);
};
