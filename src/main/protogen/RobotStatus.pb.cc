// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: RobotStatus.proto

#include "RobotStatus.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
constexpr RobotStatus::RobotStatus(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : robot_state_(0)
{}
struct RobotStatusDefaultTypeInternal {
  constexpr RobotStatusDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~RobotStatusDefaultTypeInternal() {}
  union {
    RobotStatus _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT RobotStatusDefaultTypeInternal _RobotStatus_default_instance_;
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_RobotStatus_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_RobotStatus_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_RobotStatus_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_RobotStatus_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::RobotStatus, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::RobotStatus, robot_state_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::RobotStatus)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_RobotStatus_default_instance_),
};

const char descriptor_table_protodef_RobotStatus_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\021RobotStatus.proto\"}\n\013RobotStatus\022,\n\013ro"
  "bot_state\030\001 \001(\0162\027.RobotStatus.RobotState"
  "\"@\n\nRobotState\022\014\n\010DISABLED\020\000\022\n\n\006TELEOP\020\001"
  "\022\016\n\nAUTONOMOUS\020\002\022\010\n\004TEST\020\003b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_RobotStatus_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_RobotStatus_2eproto = {
  false, false, 154, descriptor_table_protodef_RobotStatus_2eproto, "RobotStatus.proto", 
  &descriptor_table_RobotStatus_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_RobotStatus_2eproto::offsets,
  file_level_metadata_RobotStatus_2eproto, file_level_enum_descriptors_RobotStatus_2eproto, file_level_service_descriptors_RobotStatus_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_RobotStatus_2eproto_getter() {
  return &descriptor_table_RobotStatus_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_RobotStatus_2eproto(&descriptor_table_RobotStatus_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RobotStatus_RobotState_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_RobotStatus_2eproto);
  return file_level_enum_descriptors_RobotStatus_2eproto[0];
}
bool RobotStatus_RobotState_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr RobotStatus_RobotState RobotStatus::DISABLED;
constexpr RobotStatus_RobotState RobotStatus::TELEOP;
constexpr RobotStatus_RobotState RobotStatus::AUTONOMOUS;
constexpr RobotStatus_RobotState RobotStatus::TEST;
constexpr RobotStatus_RobotState RobotStatus::RobotState_MIN;
constexpr RobotStatus_RobotState RobotStatus::RobotState_MAX;
constexpr int RobotStatus::RobotState_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

class RobotStatus::_Internal {
 public:
};

RobotStatus::RobotStatus(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:RobotStatus)
}
RobotStatus::RobotStatus(const RobotStatus& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  robot_state_ = from.robot_state_;
  // @@protoc_insertion_point(copy_constructor:RobotStatus)
}

void RobotStatus::SharedCtor() {
robot_state_ = 0;
}

RobotStatus::~RobotStatus() {
  // @@protoc_insertion_point(destructor:RobotStatus)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void RobotStatus::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void RobotStatus::ArenaDtor(void* object) {
  RobotStatus* _this = reinterpret_cast< RobotStatus* >(object);
  (void)_this;
}
void RobotStatus::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RobotStatus::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void RobotStatus::Clear() {
// @@protoc_insertion_point(message_clear_start:RobotStatus)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  robot_state_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RobotStatus::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .RobotStatus.RobotState robot_state = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_robot_state(static_cast<::RobotStatus_RobotState>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* RobotStatus::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:RobotStatus)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .RobotStatus.RobotState robot_state = 1;
  if (this->_internal_robot_state() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_robot_state(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:RobotStatus)
  return target;
}

size_t RobotStatus::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:RobotStatus)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .RobotStatus.RobotState robot_state = 1;
  if (this->_internal_robot_state() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_robot_state());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData RobotStatus::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    RobotStatus::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*RobotStatus::GetClassData() const { return &_class_data_; }

void RobotStatus::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<RobotStatus *>(to)->MergeFrom(
      static_cast<const RobotStatus &>(from));
}


void RobotStatus::MergeFrom(const RobotStatus& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:RobotStatus)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_robot_state() != 0) {
    _internal_set_robot_state(from._internal_robot_state());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void RobotStatus::CopyFrom(const RobotStatus& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:RobotStatus)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RobotStatus::IsInitialized() const {
  return true;
}

void RobotStatus::InternalSwap(RobotStatus* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(robot_state_, other->robot_state_);
}

::PROTOBUF_NAMESPACE_ID::Metadata RobotStatus::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_RobotStatus_2eproto_getter, &descriptor_table_RobotStatus_2eproto_once,
      file_level_metadata_RobotStatus_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::RobotStatus* Arena::CreateMaybeMessage< ::RobotStatus >(Arena* arena) {
  return Arena::CreateMessageInternal< ::RobotStatus >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
