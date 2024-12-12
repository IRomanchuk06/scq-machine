#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_keynodes.hpp>

class Keynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_search_specified_relations{"action_search_specified_relations", ScType::NodeConstClass};

  static inline ScKeynode const action_mutation_related_entities{"action_mutation_related_entities", ScType::NodeConstClass};

  static inline ScKeynode const rrel_scq_answer{"rrel_scq_answer", ScType::NodeConstRole};
  static inline ScKeynode const rrel_scq_nested{"rrel_scq_nested", ScType::NodeConstRole};

};
