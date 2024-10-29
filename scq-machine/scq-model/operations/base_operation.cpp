#include <base_operation.hpp>

SCqBaseOperation::OperationType SCqBaseOperation::GetType() const {
    return type;
}

std::string const& SCqBaseOperation::GetName() const {
    return name;
}

std::vector<std::shared_ptr<SCqField>> const& SCqBaseOperation::GetFields() const {
    return fields;
}