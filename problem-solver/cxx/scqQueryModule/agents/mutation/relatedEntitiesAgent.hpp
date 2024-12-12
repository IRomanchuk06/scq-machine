#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/keynodes.hpp"

class MutationRelatedEntitiesAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const;

  ScResult DoProgram(ScAction & action);
};
