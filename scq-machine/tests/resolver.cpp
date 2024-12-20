#include <gtest/gtest.h>

#include "../resolver/resolver.hpp"

#include <sc-memory/test/sc_test.hpp>

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
    params.storage = "../kb.bin";

    params.user_mode = false;

    ScMemory::LogMute(); // remove for resolver
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(true); // kill
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
};

TEST_F(ScResolverTest, CreateArgsStructTest)
{
  std::string example = "QueryRelatedEntities test1 { entity_test_1 { rrel_example_2_scq { nrel_example_3_scq} nrel_example_1_scq} }";
  
  SCqTokenizer tokenizer(example);
  std::vector<Token> tokens = tokenizer.Tokenize();

  SCqParserContext parserContext(tokens);

  SCqParser parser(parserContext);
  std::shared_ptr<SCqNode> const root = parser.Parse();

  try {
    SCqResolver resolver(root);
    resolver.Resolve();
  }
  catch (utils::ScException const & e)
  {
    std::cout << e.Description() << std::endl;
  }

  SC_LOG_DEBUG("FINISH");
}