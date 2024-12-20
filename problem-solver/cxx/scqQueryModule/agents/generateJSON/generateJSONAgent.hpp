#pragma once

#include <sc-memory/sc_agent.hpp>

#include "keynodes/scq_agents_keynodes.hpp"
#include "constantsJSON.hpp"

class GenerateJSONAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const;

  ScResult DoProgram(ScAction & action);

  std::string MakeQuote(std::string const & str) const;

  std::string GenerateAnswer(ScAddr const &relsStruct ,std::string & answerJSON);

  std::string AddTabToEachLine(const std::string& input);
};
