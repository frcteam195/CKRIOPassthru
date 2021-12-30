#include "utils/DifferentialMotorConfigUpdateReporter.hpp"

DifferentialMotorConfigUpdateReporter::DifferentialMotorConfigUpdateReporter() {}

void DifferentialMotorConfigUpdateReporter::ReportAdded(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    ReportModified(message1, message2, field_path);
}

void DifferentialMotorConfigUpdateReporter::ReportModified(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    for (MessageDifferencer::SpecificField const& specificField : field_path)
    {
        if (mUpdateFunctionMap.count(specificField.field))
        {
            const ck::MotorConfiguration::Motor& m = (const ck::MotorConfiguration::Motor&)message2;
            mUpdateFunctionMap[specificField.field](m);
        }
    }
}

void DifferentialMotorConfigUpdateReporter::ReportDeleted(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path)
{
    ;   //Do nothing
}

void DifferentialMotorConfigUpdateReporter::RegisterUpdateFunction(FieldDescriptor* fieldDescriptor, std::function<void(const ck::MotorConfiguration::Motor &msg)> func)
{
    mUpdateFunctionMap[fieldDescriptor] = func; //std::move?
}