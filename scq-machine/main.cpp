#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"
#include "resolver/resolver.hpp"

#include <iostream>

static void Initialize()
{
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = false;
    params.dump_memory_statistics = false;

    params.clear = false;
    params.storage = "../kb.bin";
    params.log_level = "Debug";
    sc_char const * extensions[] = {"lib/extensions", "../bin/extensions"};
    params.extensions_directories = (const sc_char**)extensions;
    params.extensions_directories_count = 2;

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

int main()
{
    std::string example = "QueryRelatedEntities test1 { entity_test_1 { rrel_example_2_scq { nrel_example_3_scq} nrel_example_1_scq} }";

    SCqTokenizer tokenizer(example);
    std::vector<Token> tokens = tokenizer.Tokenize();

    SCqParserContext parserContext(tokens);

    SCqParser parser(parserContext);
    std::shared_ptr<SCqNode> const root = parser.Parse();

    Initialize();

    std::string result;

    try {
        SCqResolver resolver(root);
        result = resolver.Resolve();
    }
    catch (utils::ScException const & e)
    {
        std::cout << e.Description() << std::endl;
    }

    Shutdown();

    std::cout << result;

    SC_LOG_DEBUG(result);

    return 0;
}