#include "utils/DifferentialUpdateReporter.hpp"

DifferentialUpdateReporter::DifferentialUpdateReporter() {}

void DifferentialUpdateReporter::ReportAdded(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    ReportModified(message1, message2, field_path);
}

void DifferentialUpdateReporter::ReportModified(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    for (MessageDifferencer::SpecificField const& specificField : field_path)
    {
        if (mUpdateFunctionMap.count(specificField.field))
        {
            mUpdateFunctionMap[specificField.field](message2);
        }
    }
}

void DifferentialUpdateReporter::ReportDeleted(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    ;   //Do nothing
}

void DifferentialUpdateReporter::RegisterUpdateFunction(FieldDescriptor* fieldDescriptor, std::function<void(const Message &msg)> func)
{
    mUpdateFunctionMap[fieldDescriptor] = func;
}