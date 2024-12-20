#include "operation_parser.hpp"

/*

operationType operationName(optional)
{
  fieldName(argumentName: argumentValue) {
    subField1
    subField2 {
      nestedField
    }
  }
}

*/

SCqNodeType SCqOperationParser::GetOperationSCqTypeFromOperationName(std::string const &operationName)
{
    if (SCqQuery::operationNameToType.find(operationName) != SCqQuery::operationNameToType.end())
    {
        return SCqNodeType::Query;
    }
    else if (SCqMutation::operationNametoType.find(operationName) != SCqMutation::operationNametoType.end())
    {
        return SCqNodeType::Mutation;
    }
    else if (SCqSubscription::operations.find(operationName) != SCqSubscription::operations.end())
    {
        return SCqNodeType::Subscription;
    }
    else
    {
        return SCqNodeType::Field;
    }
}

std::shared_ptr<SCqNode> SCqOperationParser::Parse()
{
    context.ExpectToken(SCqTokenType::Keyword);

    SCqNodeType operationType = GetOperationSCqTypeFromOperationName(context.CurrentToken().value);

    std::shared_ptr<SCqNode> operationRoot;

    switch(operationType)
    {
        case SCqNodeType::Query: 
            operationRoot = std::make_shared<SCqQuery>(operationType, context.CurrentToken().value);
            break;

        case SCqNodeType::Mutation: 
            operationRoot = std::make_shared<SCqMutation>(operationType, context.CurrentToken().value);
            break;

        case SCqNodeType::Subscription: 
            operationRoot = std::make_shared<SCqSubscription>(operationType, context.CurrentToken().value);
            break;

        default:
            throw std::invalid_argument("Unsupported SCqNodeType: " + std::to_string(static_cast<int>(operationType)));
    }

    context.Advance(); 

    // operation name (optional)
    if(context.CurrentToken().type == SCqTokenType::Identifier)
    {
        operationRoot->value=context.CurrentToken().value;
        context.Advance();
    }

    context.ExpectToken(SCqTokenType::CurlyBraceOpen);
    context.Advance();

    while(context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
    {
        operationRoot->children.push_back(ParseEntity());
    }

    context.Advance();

    return operationRoot;
}

std::shared_ptr<SCqNode> SCqOperationParser::ParseEntity()
{
    context.ExpectToken(SCqTokenType::Identifier);
    auto entityRoot = std::make_shared<SCqNode>(SCqNodeType::Entity, context.CurrentToken().value);
    context.Advance();

    // args for entity fields
    if(context.CurrentToken().type == SCqTokenType::ParenOpen)
    {
        context.Advance();
        ParseFieldArguments();
        context.Advance();
    }

    if(context.CurrentToken().type == SCqTokenType::CurlyBraceOpen)
    {
        context.Advance();

        // handle fields for entity
        while(context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
        {
            entityRoot->children.push_back(ParseField());
        }

        context.Advance();
    }

    return entityRoot;
}

void SCqOperationParser::ParseFieldArguments()
{
    while(context.CurrentToken().type != SCqTokenType::ParenClose)
    {
        context.ExpectToken(SCqTokenType::Identifier);
        
        std::string argName = context.CurrentToken().value;
        context.Advance();

        context.ExpectToken(SCqTokenType::Colon);
        context.Advance();

        if (context.CurrentToken().type == SCqTokenType::SquareBraceOpen)
        {
            context.Advance();

            std::vector<std::string> argValues;

            while (context.CurrentToken().type != SCqTokenType::SquareBraceClose)
            {
                context.ExpectToken(SCqTokenType::StringLiteral);
                std::string argValue = context.CurrentToken().value;

                argValues.push_back(argValue);

                context.Advance();

                // skip comma 
                if(context.CurrentToken().type == SCqTokenType::Comma)
                {
                    context.Advance();
                }
            }

            arguments[argName] = argValues;

            context.Advance();
        }
        else if (context.CurrentToken().type == SCqTokenType::StringLiteral)
        {
            std::string argValue = context.CurrentToken().value;

            arguments[argName] = {argValue};

            context.Advance();
        }

        // skip comma 
        if(context.CurrentToken().type == SCqTokenType::Comma)
        {
            context.Advance();
        }
    }
}

std::shared_ptr<SCqNode> SCqOperationParser::ParseField()
{
    if(context.CurrentToken().type == SCqTokenType::Ellipsis)
    {
        context.Advance();
        context.ExpectToken(SCqTokenType::Identifier);

        std::string fragmentName = context.CurrentToken().value;
        context.Advance();

        return context.GetFragment(fragmentName);
    }

    context.ExpectToken(SCqTokenType::Identifier);
    std::string fieldName = context.CurrentToken().value;
    context.Advance();

    auto fieldRoot = std::make_shared<SCqNode>(SCqNodeType::Field, fieldName);

    fieldRoot->children.push_back(std::make_shared<SCqNode>(SCqNodeType::Argument, "ARGUMENT_LIST"));

    auto it = arguments.find(fieldName);
    if(it != arguments.end())
    {
        for (auto const & argValue : it->second)
        {
            auto const arg = std::make_shared<SCqNode>(SCqNodeType::Argument, argValue);
            fieldRoot->children[0]->children.push_back(arg);
        }
    }

    // check for nested
    if(context.CurrentToken().type == SCqTokenType::CurlyBraceOpen)
    {
        context.Advance();
        while (context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
        {
            fieldRoot->children.push_back(ParseField());
        }
        context.Advance();
    }

    return fieldRoot;
}
