#include "tokenizer.hpp"
#include <gtest/gtest.h>

TEST(TokenizerTest, TokenizeKeywords) {
    Tokenizer tokenizer("query mutation subscription");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::Keyword);
    EXPECT_EQ(tokens[0].value, "query");
    EXPECT_EQ(tokens[1].type, TokenType::Keyword);
    EXPECT_EQ(tokens[1].value, "mutation");
    EXPECT_EQ(tokens[2].type, TokenType::Keyword);
    EXPECT_EQ(tokens[2].value, "subscription");
}

TEST(TokenizerTest, TokenizeIdentifiers) {
    Tokenizer tokenizer("userName someVariable");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].value, "userName");
    EXPECT_EQ(tokens[1].type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].value, "someVariable");
}

TEST(TokenizerTest, TokenizeDirectives) {
    Tokenizer tokenizer("@include @skip");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::Directive);
    EXPECT_EQ(tokens[0].value, "include");
    EXPECT_EQ(tokens[1].type, TokenType::Directive);
    EXPECT_EQ(tokens[1].value, "skip");
}

TEST(TokenizerTest, TokenizeVariables) {
    Tokenizer tokenizer("$userId $userName");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::Variable);
    EXPECT_EQ(tokens[0].value, "userId");
    EXPECT_EQ(tokens[1].type, TokenType::Variable);
    EXPECT_EQ(tokens[1].value, "userName");
}

TEST(TokenizerTest, TokenizeNumbers) {
    Tokenizer tokenizer("123 456789");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::Number);
    EXPECT_EQ(tokens[0].value, "123");
    EXPECT_EQ(tokens[1].type, TokenType::Number);
    EXPECT_EQ(tokens[1].value, "456789");
}

TEST(TokenizerTest, TokenizeStringLiterals) {
    Tokenizer tokenizer("\"hello\" \"world\"");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].value, "hello");
    EXPECT_EQ(tokens[1].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[1].value, "world");
}

TEST(TokenizerTest, TokenizeMixedInput) {
    Tokenizer tokenizer("{ query { user(id: $userId) @include } }");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 13);
    EXPECT_EQ(tokens[0].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[1].type, TokenType::Keyword);
    EXPECT_EQ(tokens[2].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[3].type, TokenType::Identifier);
    EXPECT_EQ(tokens[4].type, TokenType::ParenOpen);
    EXPECT_EQ(tokens[5].type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].type, TokenType::Colon);
    EXPECT_EQ(tokens[7].type, TokenType::Variable);
    EXPECT_EQ(tokens[8].type, TokenType::ParenClose);
    EXPECT_EQ(tokens[9].type, TokenType::Directive);
    EXPECT_EQ(tokens[10].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[11].type, TokenType::CurlyBraceClose);
}


TEST(TokenizerTest, UnknownCharacterThrows) {
    Tokenizer tokenizer("# unknown character");
    EXPECT_THROW(tokenizer.Tokenize(), std::runtime_error);
}

TEST(TokenizerTest, IgnoreWhitespace) {
    Tokenizer tokenizer("query   mutation\n\tsubscription");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::Keyword);
    EXPECT_EQ(tokens[0].value, "query");
    EXPECT_EQ(tokens[1].type, TokenType::Keyword);
    EXPECT_EQ(tokens[1].value, "mutation");
    EXPECT_EQ(tokens[2].type, TokenType::Keyword);
    EXPECT_EQ(tokens[2].value, "subscription");
}

TEST(TokenizerTest, InvalidCharacter) {
    Tokenizer tokenizer("query # invalid character");
    EXPECT_THROW(tokenizer.Tokenize(), std::runtime_error);
}

TEST(TokenizerTest, StringEscapeSequences) {
    Tokenizer tokenizer("\"hello\\nworld\"");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[0].value, "hello\nworld");
}

TEST(TokenizerTest, EmptyInput) {
    Tokenizer tokenizer("");
    auto tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EndOfInput);
}

TEST(TokenizerTest, UnterminatedStringLiteral) {
    Tokenizer tokenizer("\"unfinished string");
    EXPECT_THROW(tokenizer.Tokenize(), std::runtime_error);
}


