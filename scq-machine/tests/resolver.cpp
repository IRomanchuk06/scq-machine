#include <gtest/gtest.h>

#include "../resolver/resolver.hpp"

#include <sc-memory/_test/sc_test.hpp>

class ScResolverTest : public ScMemoryTest
{
protected:
  virtual void SetUp()
  {
    ScResolverTest::Initialize();
    m_ctx = std::make_unique<ScAgentContext>();
  }

  virtual void TearDown()
  {
    if (m_ctx)
      m_ctx->Destroy();

    ScResolverTest::Shutdown();
  }

  static void Initialize()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = false;
    params.dump_memory_statistics = false;

    params.clear = false;
    params.repo_path = "../kb.bin";

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(false);
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
};

/*

TEST_F(ScResolverTest, RudeExample)
{
    std::vector<Token> mockTokensQuery = {
        {SCqTokenType::Keyword, "query"},
        {SCqTokenType::Identifier, "getUser"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "rude_example"},
        {SCqTokenType::CurlyBraceOpen, "{"},
        {SCqTokenType::Identifier, "rrel_example1"},
        {SCqTokenType::Identifier, "rrel_example2"},
        {SCqTokenType::CurlyBraceClose, "}"},
        {SCqTokenType::CurlyBraceClose, "}"}
    };

    auto context = SCqParserContext(mockTokensQuery);

    SCqQueryParser parser = SCqQueryParser(context);
    auto root = parser.Parse();

    auto resolver = SCqResolver(root);
    auto resStr = resolver.Resolve();
}

*/
