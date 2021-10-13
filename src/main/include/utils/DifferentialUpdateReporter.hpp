#pragma once

#include <stdint.h>
#include "IntellisenseDefs.h"
#include "google/protobuf/util/message_differencer.h"
#include <map>
#include <functional>

using namespace google::protobuf;
using namespace google::protobuf::util;

class DifferentialUpdateReporter : public MessageDifferencer::Reporter {
public:
    DifferentialUpdateReporter();
    void ReportAdded(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path) override;
    void ReportModified(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path) override;
    void ReportDeleted(const Message & message1, const Message & message2, const std::vector< MessageDifferencer::SpecificField > & field_path) override;
    void RegisterUpdateFunction(FieldDescriptor* fieldDescriptor, std::function<void(const Message &msg)> func);
private:
    std::map<const FieldDescriptor*, std::function<void(const Message &msg)>> mUpdateFunctionMap;
};
