#include "relatedEntitiesAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;

ScAddr MutationRelatedEntitiesAgent::GetActionClass() const
{
  return SCqAgentKeynodes::action_mutation_related_entities;
}

ScResult MutationRelatedEntitiesAgent::DoProgram(ScAction & action)
{
  ScAddr const & entity = action.GetArgument(1);

  if(m_context.GetElementType(entity) != ScType::NodeConst)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The first argument must be the node to search for");
  }

  ScAddr const & relsStruct = action.GetArgument(2);

  if(m_context.GetElementType(relsStruct) != ScType::NodeConstStruct)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The second argument must be a structure containing the sought relations");
  }

  ScAddr const & resultStruct = m_context.GenerateStructure();
  BuildRelsForEntity(entity, relsStruct);

  return action.FinishSuccessfully();
}

ScAddr MutationRelatedEntitiesAgent::BuildRelsForEntity(ScAddr const &entity, ScAddr const &relsStruct)
{

}
