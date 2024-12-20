/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scqQueryModule.hpp"

#include "agents/query/relatedEntitiesAgent.hpp"

#include "agents/generateJSON/generateJSONAgent.hpp"

SC_MODULE_REGISTER(SCqQueryModule)
->Agent<QueryRelatedEntitiesAgent>()
->Agent<GenerateJSONAgent>(); 
