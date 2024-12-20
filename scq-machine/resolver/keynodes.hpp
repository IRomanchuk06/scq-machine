#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_keynodes.hpp>

class SCqResolverKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const rrel_scq_argument_value{"rrel_scq_argument_value", ScType::NodeConstRole};
  static inline ScKeynode const rrel_scq_argument{"rrel_scq_argument", ScType::NodeConstRole};
  static inline ScKeynode const rrel_scq_nested{"rrel_scq_nested", ScType::NodeConstRole};

  static inline ScKeynode const action_generate_json{"action_generate_json", ScType::NodeConstClass};
  
  static inline ScKeynode const action_query_related_entities{"action_query_related_entities", ScType::NodeConstClass};
  static inline ScKeynode const action_mutation_related_entities{"action_mutation_related_entities", ScType::NodeConstClass};
};