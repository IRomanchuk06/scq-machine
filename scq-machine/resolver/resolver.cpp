#include <resolver.hpp>

using namespace SCqResolverConstants;

ScAgentContext * SCqResolver::m_context = nullptr;

std::string SCqResolver::Resolve()
{
    std::string result = "";
    result += OPEN_CURLY_BRACKETS + END_OF_LINE;

    for(auto const & operation : root->children)
    {
        SCqNodeType operationType = operation->type;

        std::string operationResult;

        if(operationType == SCqNodeType::Query)
        {
            SCqQueryResolver resolver(operation);
            operationResult = resolver.Resolve();
        }
        else if(operationType == SCqNodeType::Mutation)
        {
            SCqMutationResolver resolver(operation);
            operationResult = resolver.Resolve();
        }
        else if(operationType == SCqNodeType::Subscription)
        {
            // soon
        }

        result += AddTabToEachLine(operationResult);
    }

    result += CLOSE_CURLY_BRACKETS;

    return result;
}

std::string SCqResolver::AddTabToEachLine(const std::string &input)
{
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;

    while (std::getline(iss, line)) {
        oss << '\t' << line << '\n';
    }

    return oss.str();
}