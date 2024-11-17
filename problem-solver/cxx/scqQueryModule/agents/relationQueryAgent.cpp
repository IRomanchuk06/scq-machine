#include "relationQueryAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;

ScAddr RelationQueryAgent::GetActionClass() const
{
  return Keynodes::action_search_specified_relations;
}

ScResult RelationQueryAgent::DoProgram(ScAction & action)
{
  ScAddr const & mainNode = action.GetArgument(1);

  if(m_context.GetElementType(mainNode) != ScType::Node)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The first argument must be the node to search for");
  }

  ScAddr const & desiredRels = action.GetArgument(2);

  if(m_context.GetElementType(mainNode) != ScType::NodeStruct)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The second argument must be a structure containing the sought relations");
  }

  m_context.ForEach(
      desiredRels,
      ScType::EdgeAccessConstPosPerm,
      ScType::Node,
      [&] (ScAddr const & srcAddr, ScAddr const & edgeAddr, ScAddr const & trgAddr)
  {
    if (m_context.GetElementType(trgAddr) != ScType::NodeRole || m_context.GetElementType(trgAddr) != ScType::NodeNoRole)
    {
      SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "All elements of the second argument must be relations");
    }
  });

  return action.FinishSuccessfully();
}
 