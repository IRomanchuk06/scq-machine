#include <gtest/gtest.h>
#include <parser.hpp>

/*
    TODO: 
    1) EXPECT_EQ -> compare 2 vectors: result and expected 
    
*/ 

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

