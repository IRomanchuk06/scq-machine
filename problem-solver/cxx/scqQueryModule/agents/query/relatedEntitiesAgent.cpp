#include "relatedEntitiesAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;

ScAddr QueryRelatedEntitiesAgent::GetActionClass() const
{
  return Keynodes::action_search_specified_relations;
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

  m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, action.GetResult(), resultStruct);

  return action.FinishSuccessfully();
}

ScAddr const &QueryRelatedEntitiesAgent::FindNodesFromRelsStruct(ScAddr const &entity, ScAddr const &relsStruct, ScAddr const & resultStruct)
{
  ScIterator3Ptr it3RelNodes = m_context.CreateIterator3
  (
    relsStruct,
    ScType::EdgeAccessConstPosPerm,
    ScType::NodeConst
  );

  while (it3RelNodes->Next())
  {
    ScAddr const & node = it3RelNodes->Get(2); // rel or struct(nested fields)

    SC_LOG_DEBUG("1");
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, resultStruct, node);
    SC_LOG_DEBUG("1..");

    ScIterator5Ptr it5FindRel = m_context.CreateIterator5
    (
      entity,
      ScType::EdgeAccess, // -> or =>
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      node
    );

    while(it5FindRel->Next())
    {
      SC_LOG_DEBUG("2");
      ScAddr const & connector = m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, node, it5FindRel->Get(2));
      SC_LOG_DEBUG("3");
      m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scq_answer, connector);
    }

    ScIterator3Ptr it3CheckStruct = m_context.CreateIterator3
    (
      node,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstStruct
    );

    if(it3CheckStruct->Next())
    {
      SC_LOG_DEBUG("2");

      ScIterator5Ptr it5FindAnswers = m_context.CreateIterator5
      (
        node,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        Keynodes::rrel_scq_answer
      );
      
      while(it5FindAnswers->Next())
      {
        ScAddr const& nestedStruct = m_context.GenerateStructure();
        ScAddr const & connector = m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, node, FindNodesFromRelsStruct(it5FindAnswers->Get(2), it3CheckStruct->Get(2), nestedStruct));
        m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, Keynodes::rrel_scq_nested, connector);
      }
    }
  }

  SC_LOG_DEBUG("return");

  return resultStruct;
}