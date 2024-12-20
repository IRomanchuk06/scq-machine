#include "relatedEntitiesAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;

ScAddr QueryRelatedEntitiesAgent::GetActionClass() const
{
  return SCqAgentKeynodes::action_query_related_entities;
}

ScResult QueryRelatedEntitiesAgent::DoProgram(ScAction & action)
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
  FindNodesFromRelsStruct(entity, relsStruct, resultStruct);

  action.SetResult(resultStruct);

  return action.FinishSuccessfully();
}

ScAddr QueryRelatedEntitiesAgent::FindNodesFromRelsStruct(ScAddr const &entity, ScAddr const &relsStruct, ScAddr const & resultStruct)
{
  ScIterator5Ptr it5RelNodes = m_context.CreateIterator5
  (
    relsStruct,
    ScType::EdgeAccessConstPosPerm,
    ScType::NodeConst,
    ScType::EdgeAccessConstPosPerm, 
    SCqAgentKeynodes::rrel_scq_argument
  );

  while (it5RelNodes->Next())
  {
    ScAddr const & arg = it5RelNodes->Get(2);

    ScAddr const & connector = m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, resultStruct, arg);
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqAgentKeynodes::rrel_scq_argument, connector);

    ScIterator5Ptr it5FindRel = m_context.CreateIterator5
    (
      entity,
      ScType::Arc, // -> or =>
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      arg
    );

    while(it5FindRel->Next())
    {
      ScAddr const & connector = m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, arg, it5FindRel->Get(2));
      m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqAgentKeynodes::rrel_scq_answer, connector);
    }

    ScIterator5Ptr it5CheckStruct = m_context.CreateIterator5
    (
      arg,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstStruct,
      ScType::EdgeAccessConstPosPerm,
      SCqAgentKeynodes::rrel_scq_nested
    );

    if(it5CheckStruct->Next())
    {
      ScIterator5Ptr it5FindAnswers = m_context.CreateIterator5
      (
        arg,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        SCqAgentKeynodes::rrel_scq_answer
      );
      
      while(it5FindAnswers->Next())
      {
        ScAddr const& nested = m_context.GenerateStructure(); 
        ScAddr const& resNested = FindNodesFromRelsStruct(it5FindAnswers->Get(2), it5CheckStruct->Get(2), nested);

        m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, arg, nested);
      }
    }
  }

  return resultStruct;
}