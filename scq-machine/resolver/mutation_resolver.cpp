#include "mutation_resolver.hpp"

// Исходя из структуры дерева создаются узлы и соединяются к заданным сущностям, нужно
// не забывать учитывать корректность дуг, а также создавать элементы, которых нет в БЗ
// Смысла передавать действие агенту не вижу, тк формирование аргумента уже выполнит задачу

std::string SCqMutationResolver::Resolve()
{
    return std::string();
}

std::string SCqMutationResolver::GenerateJSON(ScAddr const &actionMutation)
{
    return std::string();
}

ScAddr SCqMutationResolver::CreateArgsStruct(std::shared_ptr<SCqNode> entity)
{
    ScAddr const & argsStruct = m_context->GenerateNode(ScType::NodeConstStruct);

    for (auto const & arg: entity->children)
    {
        ScAddr const & argAddr = m_context->SearchElementBySystemIdentifier(arg->value);

        // skip arg list in nested
        if(arg->type == SCqNodeType::Argument)
        {
            continue;
        }

        // all args have children[0] that contains the values of args
        std::vector<std::shared_ptr<SCqNode>> values = arg->children[0]->children;

        if(!values.empty())
        { 
            for(auto const & value: values)
            {
                ScAddr const & valueAddr = m_context->SearchElementBySystemIdentifier(value->value);

                ScAddr const connector = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argAddr, valueAddr);
                m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_argument_value, connector);
            }
        }

        ScAddr const & argEdge = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argsStruct, argAddr);
        m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_argument, argEdge);

        if(arg->children.size() > 1)
        {
            ScAddr const & nestedEdge = m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, argAddr, CreateArgsStruct(arg));
            m_context->GenerateConnector(ScType::EdgeAccessConstPosPerm, SCqResolverKeynodes::rrel_scq_nested, nestedEdge);
        }
    }

    return argsStruct;

    return ScAddr();
}
