/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

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

  if(m_context.GetElementType(mainNode) != ScType::NodeConst)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The first argument must be the node to search for");
  }

  ScAddr const & desiredRelsStruct = action.GetArgument(2);

  if(m_context.GetElementType(desiredRelsStruct) != ScType::NodeConstStruct)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "The second argument must be a structure containing the sought relations");
  }

  m_context.ForEach(
      desiredRelsStruct,
      ScType::EdgeAccessConstPosPerm,
      ScType::Node,
      [&] (ScAddr const & srcAddr, ScAddr const & edgeAddr, ScAddr const & trgAddr)
  {
    if (m_context.GetElementType(trgAddr) != ScType::NodeConstRole && m_context.GetElementType(trgAddr) != ScType::NodeConstNoRole)
    {
      SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "All elements of the second argument must be relations");
    }
  });

  ScAddr const & foundNodesStruct = m_context.GenerateStructure();

  m_context.ForEach(
      desiredRelsStruct,
      ScType::EdgeAccessConstPosPerm,
      ScType::Node,
      [&] (ScAddr const & srcAddr, ScAddr const & edgeAddr, ScAddr const & trgAddr)
  {
    FindNodesFromRel(mainNode, trgAddr, foundNodesStruct);
  });

  action.SetResult(foundNodesStruct);

  return action.FinishSuccessfully();
}

void RelationQueryAgent::FindNodesFromRel(ScAddr const &node, ScAddr const &rel, ScAddr const &foundNodeStruct)
{
  ScIterator5Ptr it5FindNodeFromRel = m_context.CreateIterator5
  (
    node,
    ScType::Unknown,
    ScType::Node,
    ScType::EdgeAccessConstPosPerm,
    rel
  );

  while(it5FindNodeFromRel->Next())
  {
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, foundNodeStruct, it5FindNodeFromRel->Get(2));
  }
}
