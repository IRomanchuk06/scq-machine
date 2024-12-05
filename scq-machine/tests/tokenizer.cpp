#include <gtest/gtest.h>
#include "../tokenizer/tokenizer.hpp"

TEST(SCqTokenizerTests, QueryRelatedEntities)
{
    std::string example = " MutationEntityClasses NAME { ENTITY1 { CLASS1 CLASS2 } ENTITY2 { CLASS1 } }";
    SCqTokenizer tokenizer(example);
    std::vector<Token> res = tokenizer.Tokenize();

}