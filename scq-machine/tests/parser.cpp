#include <gtest/gtest.h>
#include "../parser/parser.hpp"

#include "../tokenizer/tokenizer.hpp"


TEST(SCqParserTests, ValidQuery)
{
    std::vector<Token> query = {
        {SCqTokenType::Keyword, "query"},
        {SCqTokenType::Identifier, "getUser"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "userDetails"},
        {SCqTokenType::ParenOpen, "("},
        {SCqTokenType::Identifier, "id"}, 
        {SCqTokenType::Colon, ":"},
        {SCqTokenType::StringLiteral, "\"123\""},  
        {SCqTokenType::ParenClose, ")"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "id"},
        {SCqTokenType::Identifier, "name"},
        {SCqTokenType::CurlyBraceClose, "}"},
        {SCqTokenType::CurlyBraceClose, "}"},
        {SCqTokenType::CurlyBraceClose, "}"}
    };

    SCqParserContext context(query);

    SCqParser parser(context);
    auto root = parser.Parse()->children[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->type, SCqNodeType::Query);
    ASSERT_EQ(root->value, "getUser");

    ASSERT_EQ(root->children.size(), 1);
    ASSERT_EQ(root->children[0]->value, "userDetails");
    ASSERT_EQ(root->children[0]->type, SCqNodeType::Entity);

    ASSERT_EQ(root->children[0]->children.size(), 2);
    ASSERT_EQ(root->children[0]->children[0]->type, SCqNodeType::Field);
    ASSERT_EQ(root->children[0]->children[0]->value, "id");
    ASSERT_EQ(root->children[0]->children[0]->children[0]->type, SCqNodeType::Argument);
    ASSERT_EQ(root->children[0]->children[0]->children[0]->value, "\"123\"");

    ASSERT_EQ(root->children[0]->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(root->children[0]->children[1]->value, "name");
};

TEST(SCqParserTests, FragmentTest)
{
    std::vector<Token> query = {
        {SCqTokenType::Keyword, "fragment"},
        {SCqTokenType::Identifier, "userDetailsFragment"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "id"},
        {SCqTokenType::Identifier, "name"},
        {SCqTokenType::CurlyBraceClose, "}"},
        {SCqTokenType::Keyword, "query"},
        {SCqTokenType::Identifier, "getUser"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "user"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Ellipsis, "..."},
        {SCqTokenType::Identifier, "userDetailsFragment"},
        {SCqTokenType::CurlyBraceClose, "}"},
        {SCqTokenType::CurlyBraceClose, "}"}
    };

    SCqParserContext context(query);

    SCqParser parser(context);
    auto root = parser.Parse()->children[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->type, SCqNodeType::Query);
    ASSERT_EQ(root->value, "getUser");

    ASSERT_EQ(root->children.size(), 1);
    ASSERT_EQ(root->children[0]->value, "user");
    ASSERT_EQ(root->children[0]->type, SCqNodeType::Entity);

    ASSERT_EQ(root->children[0]->children.size(), 1);
    ASSERT_EQ(root->children[0]->children[0]->type, SCqNodeType::Fragment);
    ASSERT_EQ(root->children[0]->children[0]->value, "userDetailsFragment");

    ASSERT_EQ(root->children[0]->children[0]->children[0]->type, SCqNodeType::Field);
    ASSERT_EQ(root->children[0]->children[0]->children[0]->value, "id");
    ASSERT_EQ(root->children[0]->children[0]->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(root->children[0]->children[0]->children[1]->value, "name");
}

/*

#include <parser.hpp>

    TODO:
    1) EXPECT_EQ -> compare 2 vectors: result and expected


std::vector<Token> mockTokensQuery = {
    {TokenType::Keyword, "query"},
    {TokenType::Identifier, "getUser"},
    {TokenType::CurlyBraceOpen, "{"},
    {TokenType::Identifier, "id"},
    {TokenType::Identifier, "name"},
    {TokenType::CurlyBraceClose, "}"}
};

std::vector<Token> mockTokensMutation = {
    {TokenType::Keyword, "mutation"},
    {TokenType::Identifier, "updateUser"},
    {TokenType::CurlyBraceOpen, "{"},
    {TokenType::Identifier, "id"},
    {TokenType::Identifier, "name"},
    {TokenType::CurlyBraceClose, "}"}
};

std::vector<Token> mockTokensSubscription = {
    {TokenType::Keyword, "subscription"},
    {TokenType::Identifier, "userUpdated"},
    {TokenType::CurlyBraceOpen, "{"},
    {TokenType::Identifier, "id"},
    {TokenType::Identifier, "name"},
    {TokenType::CurlyBraceClose, "}"}
};

TEST(ParseTest, ValidQuery) {
    SCqParser parser(mockTokensQuery);
    auto root = parser.Parse();

    ASSERT_EQ(root->children.size(), 1);
    auto queryNode = root->children[0];
    ASSERT_EQ(queryNode->type, SCqNodeType::Query);
    ASSERT_EQ(queryNode->value, "getUser");
    ASSERT_EQ(queryNode->children.size(), 2);

    ASSERT_EQ(queryNode->children[0]->type, SCqNodeType::Field);
    ASSERT_EQ(queryNode->children[0]->value, "id");

    ASSERT_EQ(queryNode->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(queryNode->children[1]->value, "name");
}

TEST(ParseTest, ValidMutation) {
    SCqParser parser(mockTokensMutation);
    auto root = parser.Parse();

    ASSERT_EQ(root->children.size(), 1);
    auto mutationNode = root->children[0];
    ASSERT_EQ(mutationNode->type, SCqNodeType::Mutation);
    ASSERT_EQ(mutationNode->value, "updateUser");
    ASSERT_EQ(mutationNode->children.size(), 2);

    ASSERT_EQ(mutationNode->children[0]->type, SCqNodeType::Field);
    ASSERT_EQ(mutationNode->children[0]->value, "id");

    ASSERT_EQ(mutationNode->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(mutationNode->children[1]->value, "name");
}

TEST(ParseTest, ValidSubscription) {
    SCqParser parser(mockTokensSubscription);
    auto root = parser.Parse();

    ASSERT_EQ(root->children.size(), 1);
    auto subscriptionNode = root->children[0];
    ASSERT_EQ(subscriptionNode->type, SCqNodeType::Subscription);
    ASSERT_EQ(subscriptionNode->value, "userUpdated");
    ASSERT_EQ(subscriptionNode->children.size(), 2);

    ASSERT_EQ(subscriptionNode->children[0]->type, SCqNodeType::Field);
    ASSERT_EQ(subscriptionNode->children[0]->value, "id");

    ASSERT_EQ(subscriptionNode->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(subscriptionNode->children[1]->value, "name");
}

TEST(ParseTest, InvalidQueryMissingCurlyBrace) {
    std::vector<Token> tokens = {
        {TokenType::Keyword, "query"},
        {TokenType::Identifier, "getUser"},
        {TokenType::Identifier, "id"},
        {TokenType::Identifier, "name"}
    };

    SCqParser parser(tokens);

    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

TEST(ParseTest, InvalidTokenUnexpected) {
    std::vector<Token> tokens = {
        {TokenType::Keyword, "query"},
        {TokenType::Identifier, "getUser"},
        {TokenType::CurlyBraceOpen, "{"},
        {TokenType::Identifier, "id"},
        {TokenType::Keyword, "mutation"}
    };

    SCqParser parser(tokens);

    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

TEST(ParseTest, EmptyInput) {
    std::vector<Token> tokens = {};

    SCqParser parser(tokens);

    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

TEST(ParseTest, QueryWithArguments) {
    std::vector<Token> tokens = {
        {TokenType::Keyword, "query"},
        {TokenType::Identifier, "getUser"},
        {TokenType::ParenOpen, "("},
        {TokenType::Identifier, "id"},
        {TokenType::Colon, ":"},
        {TokenType::Number, "1"},
        {TokenType::Comma, ","},
        {TokenType::Identifier, "name"},
        {TokenType::Colon, ":"},
        {TokenType::StringLiteral, "\"John\""},
        {TokenType::ParenClose, ")"},
        {TokenType::CurlyBraceOpen, "{"},
        {TokenType::Identifier, "id"},
        {TokenType::Identifier, "name"},
        {TokenType::CurlyBraceClose, "}"}
    };

    SCqParser parser(tokens);
    auto root = parser.Parse();

    ASSERT_EQ(root->children.size(), 1);
    auto queryNode = root->children[0];
    ASSERT_EQ(queryNode->type, SCqNodeType::Query);
    ASSERT_EQ(queryNode->value, "getUser");

    ASSERT_EQ(queryNode->children.size(), 3);

    auto argumentsNode = queryNode->children[0];
    ASSERT_EQ(argumentsNode->type, SCqNodeType::Argument);
    ASSERT_EQ(argumentsNode->children.size(), 2);

    auto arg1 = argumentsNode->children[0];
    auto arg2 = argumentsNode->children[1];

    ASSERT_EQ(arg1->type, SCqNodeType::Argument);
    ASSERT_EQ(arg1->children.size(), 2);

    ASSERT_EQ(arg1->children[0]->type, SCqNodeType::Identifier);
    ASSERT_EQ(arg1->children[0]->value, "id");
    ASSERT_EQ(arg1->children[1]->type, SCqNodeType::Number);
    ASSERT_EQ(arg1->children[1]->value, "1");

    ASSERT_EQ(arg2->type, SCqNodeType::Argument);
    ASSERT_EQ(arg2->children.size(), 2);

    ASSERT_EQ(arg2->children[0]->type, SCqNodeType::Identifier);
    ASSERT_EQ(arg2->children[0]->value, "name");
    ASSERT_EQ(arg2->children[1]->type, SCqNodeType::StringLiteral);
    ASSERT_EQ(arg2->children[1]->value, "\"John\"");

    ASSERT_EQ(queryNode->children[1]->type, SCqNodeType::Field);
    ASSERT_EQ(queryNode->children[1]->value, "id");

    ASSERT_EQ(queryNode->children[2]->type, SCqNodeType::Field);
    ASSERT_EQ(queryNode->children[2]->value, "name");
}

First, the basic version without the fragment

TEST(SCqParserTests, ParseQueryWithFragmentUsage) {
    std::vector<Token> tokens = {
        {TokenType::Keyword, "fragment"},
        {TokenType::Identifier, "UserFragment"},
        {TokenType::Keyword, "on"},
        {TokenType::Identifier, "User"},
        {TokenType::CurlyBraceOpen, "{"},
        {TokenType::Identifier, "id"},
        {TokenType::Identifier, "name"},
        {TokenType::CurlyBraceClose, "}"},
        {TokenType::Keyword, "query"},
        {TokenType::Identifier, "GetUsers"},
        {TokenType::CurlyBraceOpen, "{"},
        {TokenType::Identifier, "users"},
        {TokenType::CurlyBraceOpen, "{"},
        {TokenType::Identifier, "...UserFragment"},
        {TokenType::CurlyBraceClose, "}"},
        {TokenType::CurlyBraceClose, "}"},
    };

    SCqParser parser(tokens);
    auto rootNode = parser.Parse();

    ASSERT_EQ(rootNode->type, SCqNodeType::Query);
    ASSERT_EQ(rootNode->children.size(), 2);

    auto fragmentNode = rootNode->children[0];
    ASSERT_EQ(fragmentNode->type, SCqNodeType::Fragment);
    ASSERT_EQ(fragmentNode->value, "UserFragment");
    ASSERT_EQ(fragmentNode->children.size(), 3);

    auto typeNode = fragmentNode->children[0];
    ASSERT_EQ(typeNode->type, SCqNodeType::Type);
    ASSERT_EQ(typeNode->value, "User");

    auto field1 = fragmentNode->children[1];
    ASSERT_EQ(field1->type, SCqNodeType::Field);
    ASSERT_EQ(field1->value, "id");

    auto field2 = fragmentNode->children[2];
    ASSERT_EQ(field2->type, SCqNodeType::Field);
    ASSERT_EQ(field2->value, "name");

    auto queryNode = rootNode->children[1];
    ASSERT_EQ(queryNode->type, SCqNodeType::Query);
    ASSERT_EQ(queryNode->value, "GetUsers");
    ASSERT_EQ(queryNode->children.size(), 1);

    auto usersNode = queryNode->children[0];
    ASSERT_EQ(usersNode->type, SCqNodeType::Field);
    ASSERT_EQ(usersNode->value, "users");
    ASSERT_EQ(usersNode->children.size(), 1);

    auto fragmentUsageNode = usersNode->children[0];
    ASSERT_EQ(fragmentUsageNode->type, SCqNodeType::Field);
    ASSERT_EQ(fragmentUsageNode->value, "...UserFragment");
}


*/

