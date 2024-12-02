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

std::shared_ptr<SCqNode> SCqOperationParser::Parse(SCqParserContext &context)
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
        ParseEntity(context);
    }

    return nullptr;
}

SCqNodeType SCqOperationParser::GetOperationSCqTypeFromOperationName(std::string const &operationName)
{
    if(operationName == "Query")
    {
        return SCqNodeType::Query;
    }
    else if(operationName == "Mutation")
    {
        return SCqNodeType::Mutation;
    }
    else if(operationName == "Subscription")
    {
        return SCqNodeType::Subscription;
    }
    else
    {
        return;
    }
}

std::shared_ptr<SCqNode> SCqOperationParser::ParseEntity(SCqParserContext &context)
{
    context.ExpectToken(SCqTokenType::Identifier);
    auto entityRoot = std::make_shared<SCqNode>(SCqNodeType::Entity, context.CurrentToken().value);
    context.Advance();

    // args for entity fields
    if(context.CurrentToken().type == SCqTokenType::ParenOpen)
    {
        ParseFieldArguments(context);
    }

    context.ExpectToken(SCqTokenType::CurlyBraceOpen);
    context.Advance();

    // handle fields for entity
    while(context.CurrentToken().type != SCqTokenType::CurlyBraceClose)
    {
        ParseField(context);
    }

    context.Advance();

    return entityRoot;
}

void SCqOperationParser::ParseFieldArguments(SCqParserContext &context)
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

        arguments[argName] = argValue;

        // skip comma 
        if(context.CurrentToken().type == SCqTokenType::Comma)
        {
            context.Advance();
        }
    }

    context.Advance();
}

std::shared_ptr<SCqNode> SCqOperationParser::ParseField(SCqParserContext &context)
{
    context.ExpectToken(SCqTokenType::Identifier);

    std::string fieldName = context.CurrentToken().value;

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
            fieldRoot->children.push_back(ParseField(context));
        }   
        context.Advance();
    }

    return fieldRoot;
}
