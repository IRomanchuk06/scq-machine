#include "generateJSONAgent.hpp"

#include <iostream>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_stream.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

using namespace std;
using namespace utils;
using namespace JSONConstants;

ScAddr GenerateJSONAgent::GetActionClass() const
{
  return SCqAgentKeynodes::action_generate_json;
}

ScResult GenerateJSONAgent::DoProgram(ScAction & action)
{
	ScAddr const & input = action.GetArgument(1);

	std::string resultJSON = "";
	
	std::string entityName = "";

	ScIterator5Ptr it5FindEntityName = m_context.CreateIterator5
	(
		input,
		ScType::EdgeAccessConstPosPerm,
		ScType::NodeConst,
		ScType::EdgeAccessConstPosPerm,
		ScKeynodes::rrel_1
	);

	if(it5FindEntityName->Next())
	{
		entityName = m_context.GetElementSystemIdentifier(it5FindEntityName->Get(2));
	}

	ScIterator5Ptr it5FindAnswerStruct = m_context.CreateIterator5
	(
		input,
		ScType::EdgeDCommonConst,
		ScType::NodeConst,
		ScType::EdgeAccessConstPosPerm,
		ScKeynodes::nrel_result
	);

	ScAddr agentAnswerStruct;

	if(it5FindAnswerStruct->Next())
	{
		agentAnswerStruct = it5FindAnswerStruct->Get(2);
	}

	std::string answerJSON = "";
	GenerateAnswer(agentAnswerStruct , answerJSON);

	resultJSON += MakeQuote(entityName) + SPACE + COLON + SPACE + OPEN_CURLY_BRACKETS + END_OF_LINE + AddTabToEachLine(answerJSON) + CLOSE_CURLY_BRACKETS;

	ScAddr const & resNode = m_context.GenerateLink();

	m_context.SetLinkContent(resNode, resultJSON);

	action.SetResult(resNode);

 	return action.FinishSuccessfully();
}

std::string GenerateJSONAgent::GenerateAnswer(ScAddr const &relsStruct ,std::string & answerJSON)
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

		std::string const argName = m_context.GetElementSystemIdentifier(arg);
		answerJSON += MakeQuote(argName) + SPACE + COLON + SPACE;

		ScIterator5Ptr it5RelAnswer = m_context.CreateIterator5
		(
			arg,
			ScType::Arc, // -> or =>
			ScType::NodeConst,
			ScType::EdgeAccessConstPosPerm,
			SCqAgentKeynodes::rrel_scq_answer
		);

		std::vector<std::string> relAnswers;

		while(it5RelAnswer->Next())
		{
			relAnswers.push_back(MakeQuote(m_context.GetElementSystemIdentifier(it5RelAnswer->Get(2))));
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
				std::string nested = "";
				GenerateAnswer(it5CheckStruct->Get(2), nested);
				
				answerJSON += OPEN_CURLY_BRACKETS + END_OF_LINE + AddTabToEachLine(nested) + CLOSE_CURLY_BRACKETS;
			}
		}
		else
		{
			if(relAnswers.size() > 1)
			{
				answerJSON += OPEN_BRACKET;

				for (size_t i = 0; i < relAnswers.size(); ++i)
				{
					answerJSON += relAnswers[i];
					if (i < relAnswers.size() - 1)
					{
						answerJSON += COMMA + SPACE;
					}
				}

				answerJSON += CLOSE_BRACKET;
			}
			else
			{
				answerJSON += relAnswers[0];
			}
		}
		answerJSON += END_OF_LINE;
	}

    return answerJSON;
}

std::string GenerateJSONAgent::AddTabToEachLine(const std::string &input)
{
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;

    while (std::getline(iss, line)) {
        oss << '\t' << line << '\n';
    }

    return oss.str();
}

std::string GenerateJSONAgent::MakeQuote(std::string const &str) const
{
	return QUOTES + str + QUOTES;
}