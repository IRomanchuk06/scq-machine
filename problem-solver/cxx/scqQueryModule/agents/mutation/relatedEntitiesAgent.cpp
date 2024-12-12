#include "relatedEntitiesAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;

ScAddr MutationRelatedEntitiesAgent::GetActionClass() const
{
  return Keynodes::action_mutation_related_entities;
}

ScResult MutationRelatedEntitiesAgent::DoProgram(ScAction & action)
{
  /*
  ScIterator5Ptr it5CheckRels = m_context.Iterator5
  (
    node,
    ScType::EdgeAccessConstPosPerm,
    ScType::NodeConst,
    ScType::EdgeAccessConstPosPerm,
    SCqKeynodes::rrel_scq_argument_value
  );
  */

  return action.FinishSuccessfully();
}