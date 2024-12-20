#include <gtest/gtest.h>

#include "../parser/parser.hpp" 
#include "../tokenizer/tokenizer.hpp"

TEST(SCqParserTests, MutationRelatedEntities)
{
    std::string example = "MutationRelatedEntities NAME { ENTITY (REL1 : [\"ARGUMENT1\", \"ARGUMENT2\"], REL2 : \"ARGUMENT3\", REL3 : \"ARGUMENT4\") { REL1 REL2 { REL3	} } }";
    
    SCqTokenizer tokenizer(example);
    std::vector<Token> tokens = tokenizer.Tokenize();

    SCqParserContext parserContext(tokens);

    SCqParser parser(parserContext);
    std::shared_ptr<SCqNode> const root = parser.Parse();

    EXPECT_EQ(root->children.size(), 1);

    auto operationNode = root->children[0];

    auto mutationOperation = std::static_pointer_cast<SCqMutation>(operationNode);

    EXPECT_EQ(mutationOperation->type, SCqNodeType::Mutation);
    EXPECT_EQ(mutationOperation->value, "NAME");
    EXPECT_EQ(mutationOperation->operationType, SCqMutation::SCqMutationType::MutationRelatedEntities);


    EXPECT_EQ(root->children[0]->children.size(), 1);
    EXPECT_EQ(root->children[0]->children[0]->value, "ENTITY");

}