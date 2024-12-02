#include "operation_parser.hpp"

/*

operationType operationName(optional) ($variables(optional): VariableType)
{
  fieldName(argumentName: argumentValue) {
    subField1
    subField2 {
      nestedField
    }
  }
}

*/

// TODO: add vars handling

std::shared_ptr<SCqNode> SCqOperationParser::Parse()
{
    context.ExpectToken(SCqTokenType::Keyword);
    auto operationRoot = std::make_shared<SCqNode>(GetOperationSCqTypeFromOperationName(context.CurrentToken().value));
    context.Advance(); 

    // operation name (optional)
    if(context.CurrentToken().type == SCqTokenType::Identifier)
    {
        operationRoot->value=context.CurrentToken().value;
        context.Advance();
    }

    // operation vars (optional)
    if(context.CurrentToken().type == SCqTokenType::ParenOpen)
    {
        // func to handle vars
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

SCqNodeType SCqOperationParser::GetOperationSCqTypeFromOperationName(std::string const &operationName)
{
    std::string lowerOperationName = operationName;
    std::transform(lowerOperationName.begin(), lowerOperationName.end(), lowerOperationName.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (lowerOperationName == "query")
    {
        return SCqNodeType::Query;
    }
    else if (lowerOperationName == "mutation")
    {
        return SCqNodeType::Mutation;
    }
    else if (lowerOperationName == "subscription")
    {
        return SCqNodeType::Subscription;
    }
    else
    {
        return SCqNodeType::Field;
    }
}

std::shared_ptr<SCqNode> SCqOperationParser::ParseEntity()
{
    context.ExpectToken(SCqTokenType::Identifier);
    auto entityRoot = std::make_shared<SCqNode>(SCqNodeType::Entity, context.CurrentToken().value);
    context.Advance();

    // args for entity fields
    if(context.CurrentToken().type == SCqTokenType::ParenOpen)
    {
        ParseFieldArguments();
    }

    context.ExpectToken(SCqTokenType::CurlyBraceOpen);
    context.Advance();

    // handle fields for entity
    while(context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
    {
        entityRoot->children.push_back(ParseField());
    }

    context.Advance();

    return entityRoot;
}

void SCqOperationParser::ParseFieldArguments()
{
    context.Advance();

    while(context.CurrentToken().type != SCqTokenType::ParenClose)
    {
        context.ExpectToken(SCqTokenType::Identifier);
        
        std::string argName = context.CurrentToken().value;
        context.Advance();

        context.ExpectToken(SCqTokenType::Colon);
        context.Advance();
        context.ExpectToken(SCqTokenType::StringLiteral);

        std::string argValue = context.CurrentToken().value;
        context.Advance();

        arguments[argName] = argValue;

        // skip comma 
        if(context.CurrentToken().type == SCqTokenType::Comma)
        {
            context.Advance();
        }
    }
    context.Advance();
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

    auto it = arguments.find(fieldName);
    if(it != arguments.end())
    {
        fieldRoot->children.push_back(std::make_shared<SCqNode>(SCqNodeType::Argument, it->second));
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
