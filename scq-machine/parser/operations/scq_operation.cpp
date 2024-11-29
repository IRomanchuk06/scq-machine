#include "scq_operation.hpp"

void SCqOperation::AddNested(std::shared_ptr<SCqNode> nested)
{
    nestedOperations.push_back(nested);
}