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
      params.log_level = "Debug";
      sc_char const * extensions[] = {"../bin/extensions"};
      params.extensions_directories = (const sc_char**)extensions;
      params.extensions_directories_count = 1;

      params.user_mode = false;

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

TEST_F(ScResolverTest, QueryRelatedEntities)
{
  std::string example = "QueryRelatedEntities test1 { entity_test_1 { rrel_example_2_scq { nrel_example_3_scq} nrel_example_1_scq} }";

  SCqTokenizer tokenizer(example);
  std::vector<Token> tokens = tokenizer.Tokenize();

  SCqParserContext parserContext(tokens);

  SCqParser parser(parserContext);
  std::shared_ptr<SCqNode> const root = parser.Parse();


  std::string result;

  try {
      //SCqResolver resolver(root);
      //result = resolver.Resolve();
  }
  catch (utils::ScException const & e)
  {
      std::cout << e.Description() << std::endl;
  }

  std::cout << result;


  EXPECT_EQ(result, "{\n\t\"QueryRelatedEntities_test1\" : {\n\t\t\"entity_test_1\" : {\n\t\t\t\"rrel_example_2_scq\" : {\n\t\t\t\t\"nrel_example_3_scq\" : \"ex_3\"\n\t\t\t}\n\t\t\t\"nrel_example_1_scq\" : \"ex_1\"\n\t\t}\n\t\n\t}\n}");
}