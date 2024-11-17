#include <gtest/gtest.h>
#include "tokenizer.hpp"

/*
    TODO: 
    1) EXPECT_EQ -> compare 2 vectors: result and expected 
    2) line items in a separate file ?

*/ 

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

TEST(TokenizerTest, MutationWithVariables) {
    std::string source = R"(mutation CreateNewPost($title: String!) { createPost(input: { title: $title }) { id title } })";
    Tokenizer tokenizer(source);

    std::vector<Token> tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 25);

    EXPECT_EQ(tokens[0].type, TokenType::Keyword);
    EXPECT_EQ(tokens[0].value, "mutation");

    EXPECT_EQ(tokens[1].type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].value, "CreateNewPost");

    EXPECT_EQ(tokens[2].type, TokenType::ParenOpen);
    EXPECT_EQ(tokens[2].value, "(");

    EXPECT_EQ(tokens[3].type, TokenType::Variable);
    EXPECT_EQ(tokens[3].value, "title");

    EXPECT_EQ(tokens[4].type, TokenType::Colon);
    EXPECT_EQ(tokens[4].value, ":");

    EXPECT_EQ(tokens[5].type, TokenType::OutputType);
    EXPECT_EQ(tokens[5].value, "String");

    EXPECT_EQ(tokens[6].type, TokenType::Exclamation);
    EXPECT_EQ(tokens[6].value, "!");

    EXPECT_EQ(tokens[7].type, TokenType::ParenClose);
    EXPECT_EQ(tokens[7].value, ")");

    EXPECT_EQ(tokens[8].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[8].value, "{");

    EXPECT_EQ(tokens[9].type, TokenType::Identifier);
    EXPECT_EQ(tokens[9].value, "createPost");

    EXPECT_EQ(tokens[10].type, TokenType::ParenOpen);
    EXPECT_EQ(tokens[10].value, "(");

    EXPECT_EQ(tokens[11].type, TokenType::Identifier);
    EXPECT_EQ(tokens[11].value, "input");

    EXPECT_EQ(tokens[12].type, TokenType::Colon);
    EXPECT_EQ(tokens[12].value, ":");

    EXPECT_EQ(tokens[13].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[13].value, "{");

    EXPECT_EQ(tokens[14].type, TokenType::Identifier);
    EXPECT_EQ(tokens[14].value, "title");

    EXPECT_EQ(tokens[15].type, TokenType::Colon);
    EXPECT_EQ(tokens[15].value, ":");

    EXPECT_EQ(tokens[16].type, TokenType::Variable);
    EXPECT_EQ(tokens[16].value, "title");

    EXPECT_EQ(tokens[17].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[17].value, "}");

    EXPECT_EQ(tokens[18].type, TokenType::ParenClose);
    EXPECT_EQ(tokens[18].value, ")");

    EXPECT_EQ(tokens[19].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[19].value, "{");

    EXPECT_EQ(tokens[20].type, TokenType::Identifier);
    EXPECT_EQ(tokens[20].value, "id");

    EXPECT_EQ(tokens[21].type, TokenType::Identifier);
    EXPECT_EQ(tokens[21].value, "title");

    EXPECT_EQ(tokens[22].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[22].value, "}");

    EXPECT_EQ(tokens[23].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[23].value, "}");

    EXPECT_EQ(tokens[24].type, TokenType::EndOfInput);
    EXPECT_EQ(tokens[24].value, "");
}

TEST(TokenizerTest, Subscription) {
    std::string source = R"(subscription OnNewComment { commentAdded(postId: "123") { id text author { name } } })";
    Tokenizer tokenizer(source);

    std::vector<Token> tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 19);

    EXPECT_EQ(tokens[0].type, TokenType::Keyword);
    EXPECT_EQ(tokens[0].value, "subscription");

    EXPECT_EQ(tokens[1].type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].value, "OnNewComment");

    EXPECT_EQ(tokens[2].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[2].value, "{");

    EXPECT_EQ(tokens[3].type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].value, "commentAdded");

    EXPECT_EQ(tokens[4].type, TokenType::ParenOpen);
    EXPECT_EQ(tokens[4].value, "(");

    EXPECT_EQ(tokens[5].type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].value, "postId");

    EXPECT_EQ(tokens[6].type, TokenType::Colon);
    EXPECT_EQ(tokens[6].value, ":");

    EXPECT_EQ(tokens[7].type, TokenType::StringLiteral);
    EXPECT_EQ(tokens[7].value, "123");

    EXPECT_EQ(tokens[8].type, TokenType::ParenClose);
    EXPECT_EQ(tokens[8].value, ")");

    EXPECT_EQ(tokens[9].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[9].value, "{");

    EXPECT_EQ(tokens[10].type, TokenType::Identifier);
    EXPECT_EQ(tokens[10].value, "id");

    EXPECT_EQ(tokens[11].type, TokenType::Identifier);
    EXPECT_EQ(tokens[11].value, "text");

    EXPECT_EQ(tokens[12].type, TokenType::Identifier);
    EXPECT_EQ(tokens[12].value, "author");

    EXPECT_EQ(tokens[13].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[13].value, "{");

    EXPECT_EQ(tokens[14].type, TokenType::Identifier);
    EXPECT_EQ(tokens[14].value, "name");

    EXPECT_EQ(tokens[15].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[15].value, "}");

    EXPECT_EQ(tokens[16].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[16].value, "}");

    EXPECT_EQ(tokens[17].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[17].value, "}");

    EXPECT_EQ(tokens[18].type, TokenType::EndOfInput);
    EXPECT_EQ(tokens[18].value, "");
}

TEST(TokenizerTest, Fragment) {
    std::string source = R"(fragment PostDetails on Post {id title author {name} } )";

    Tokenizer tokenizer(source);

    std::vector<Token> tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 13); 

    EXPECT_EQ(tokens[0].type, TokenType::Keyword);
    EXPECT_EQ(tokens[0].value, "fragment");

    EXPECT_EQ(tokens[1].type, TokenType::Identifier);
    EXPECT_EQ(tokens[1].value, "PostDetails");

    EXPECT_EQ(tokens[2].type, TokenType::Keyword);
    EXPECT_EQ(tokens[2].value, "on");

    EXPECT_EQ(tokens[3].type, TokenType::Identifier);
    EXPECT_EQ(tokens[3].value, "Post");

    EXPECT_EQ(tokens[4].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[4].value, "{");

    EXPECT_EQ(tokens[5].type, TokenType::Identifier);
    EXPECT_EQ(tokens[5].value, "id");

    EXPECT_EQ(tokens[6].type, TokenType::Identifier);
    EXPECT_EQ(tokens[6].value, "title");

    EXPECT_EQ(tokens[7].type, TokenType::Identifier);
    EXPECT_EQ(tokens[7].value, "author");

    EXPECT_EQ(tokens[8].type, TokenType::CurlyBraceOpen);
    EXPECT_EQ(tokens[8].value, "{");

    EXPECT_EQ(tokens[9].type, TokenType::Identifier);
    EXPECT_EQ(tokens[9].value, "name");

    EXPECT_EQ(tokens[10].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[10].value, "}");

    EXPECT_EQ(tokens[11].type, TokenType::CurlyBraceClose);
    EXPECT_EQ(tokens[11].value, "}");

    EXPECT_EQ(tokens[12].type, TokenType::EndOfInput);
    EXPECT_EQ(tokens[12].value, "");
}

TEST(TokenizerTest, QueryWithFragments) {
    std::string source = R"(
        query GetPostDetails {
          posts {
            ...PostDetails
            comments {
              ...CommentDetails
            }
          }
        }

        fragment PostDetails on Post {
          id
          title
          content
        }

        fragment CommentDetails on Comment {
          id
          text
          author {
            name
          }
        }
    )";

    Tokenizer tokenizer(source);

    std::vector<Token> tokens = tokenizer.Tokenize();

    ASSERT_EQ(tokens.size(), 36); 

    ASSERT_EQ(tokens[0].type, TokenType::Keyword);
    ASSERT_EQ(tokens[0].value, "query");

    ASSERT_EQ(tokens[1].type, TokenType::Identifier);
    ASSERT_EQ(tokens[1].value, "GetPostDetails");

    ASSERT_EQ(tokens[2].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[2].value, "{");

    ASSERT_EQ(tokens[3].type, TokenType::Identifier);
    ASSERT_EQ(tokens[3].value, "posts");

    ASSERT_EQ(tokens[4].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[4].value, "{");

    ASSERT_EQ(tokens[5].type, TokenType::Ellipsis);
    ASSERT_EQ(tokens[5].value, "...");

    ASSERT_EQ(tokens[6].type, TokenType::Identifier);
    ASSERT_EQ(tokens[6].value, "PostDetails");

    ASSERT_EQ(tokens[7].type, TokenType::Identifier);
    ASSERT_EQ(tokens[7].value, "comments");

    ASSERT_EQ(tokens[8].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[8].value, "{");

    ASSERT_EQ(tokens[9].type, TokenType::Ellipsis);
    ASSERT_EQ(tokens[9].value, "...");

    ASSERT_EQ(tokens[10].type, TokenType::Identifier);
    ASSERT_EQ(tokens[10].value, "CommentDetails");

    ASSERT_EQ(tokens[11].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[11].value, "}");

    ASSERT_EQ(tokens[12].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[12].value, "}");

    ASSERT_EQ(tokens[13].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[13].value, "}");

    ASSERT_EQ(tokens[14].type, TokenType::Keyword);
    ASSERT_EQ(tokens[14].value, "fragment");

    ASSERT_EQ(tokens[15].type, TokenType::Identifier);
    ASSERT_EQ(tokens[15].value, "PostDetails");

    ASSERT_EQ(tokens[16].type, TokenType::Keyword);
    ASSERT_EQ(tokens[16].value, "on");

    ASSERT_EQ(tokens[17].type, TokenType::Identifier);
    ASSERT_EQ(tokens[17].value, "Post");

    ASSERT_EQ(tokens[18].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[18].value, "{");

    ASSERT_EQ(tokens[19].type, TokenType::Identifier);
    ASSERT_EQ(tokens[19].value, "id");

    ASSERT_EQ(tokens[20].type, TokenType::Identifier);
    ASSERT_EQ(tokens[20].value, "title");

    ASSERT_EQ(tokens[21].type, TokenType::Identifier);
    ASSERT_EQ(tokens[21].value, "content");

    ASSERT_EQ(tokens[22].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[22].value, "}");

    ASSERT_EQ(tokens[23].type, TokenType::Keyword);
    ASSERT_EQ(tokens[23].value, "fragment");

    ASSERT_EQ(tokens[24].type, TokenType::Identifier);
    ASSERT_EQ(tokens[24].value, "CommentDetails");

    ASSERT_EQ(tokens[25].type, TokenType::Keyword);
    ASSERT_EQ(tokens[25].value, "on");

    ASSERT_EQ(tokens[26].type, TokenType::Identifier);
    ASSERT_EQ(tokens[26].value, "Comment");

    ASSERT_EQ(tokens[27].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[27].value, "{");

    ASSERT_EQ(tokens[28].type, TokenType::Identifier);
    ASSERT_EQ(tokens[28].value, "id");

    ASSERT_EQ(tokens[29].type, TokenType::Identifier);
    ASSERT_EQ(tokens[29].value, "text");

    ASSERT_EQ(tokens[30].type, TokenType::Identifier);
    ASSERT_EQ(tokens[30].value, "author");

    ASSERT_EQ(tokens[31].type, TokenType::CurlyBraceOpen);
    ASSERT_EQ(tokens[31].value, "{");

    ASSERT_EQ(tokens[32].type, TokenType::Identifier);
    ASSERT_EQ(tokens[32].value, "name"); 

    ASSERT_EQ(tokens[33].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[33].value, "}");

    ASSERT_EQ(tokens[34].type, TokenType::CurlyBraceClose);
    ASSERT_EQ(tokens[34].value, "}");

    ASSERT_EQ(tokens[35].type, TokenType::EndOfInput);
    ASSERT_EQ(tokens[35].value, "");
}
