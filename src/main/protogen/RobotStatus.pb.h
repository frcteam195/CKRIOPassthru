// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: RobotStatus.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_RobotStatus_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_RobotStatus_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3018000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3018000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_RobotStatus_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_RobotStatus_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_RobotStatus_2eproto;
class RobotStatus;
struct RobotStatusDefaultTypeInternal;
extern RobotStatusDefaultTypeInternal _RobotStatus_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::RobotStatus* Arena::CreateMaybeMessage<::RobotStatus>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum RobotStatus_RobotState : int {
  RobotStatus_RobotState_DISABLED = 0,
  RobotStatus_RobotState_TELEOP = 1,
  RobotStatus_RobotState_AUTONOMOUS = 2,
  RobotStatus_RobotState_TEST = 3,
  RobotStatus_RobotState_RobotStatus_RobotState_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  RobotStatus_RobotState_RobotStatus_RobotState_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool RobotStatus_RobotState_IsValid(int value);
constexpr RobotStatus_RobotState RobotStatus_RobotState_RobotState_MIN = RobotStatus_RobotState_DISABLED;
constexpr RobotStatus_RobotState RobotStatus_RobotState_RobotState_MAX = RobotStatus_RobotState_TEST;
constexpr int RobotStatus_RobotState_RobotState_ARRAYSIZE = RobotStatus_RobotState_RobotState_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RobotStatus_RobotState_descriptor();
template<typename T>
inline const std::string& RobotStatus_RobotState_Name(T enum_t_value) {
  static_assert(::std::is_same<T, RobotStatus_RobotState>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function RobotStatus_RobotState_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    RobotStatus_RobotState_descriptor(), enum_t_value);
}
inline bool RobotStatus_RobotState_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, RobotStatus_RobotState* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<RobotStatus_RobotState>(
    RobotStatus_RobotState_descriptor(), name, value);
}
// ===================================================================

class RobotStatus final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RobotStatus) */ {
 public:
  inline RobotStatus() : RobotStatus(nullptr) {}
  ~RobotStatus() override;
  explicit constexpr RobotStatus(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  RobotStatus(const RobotStatus& from);
  RobotStatus(RobotStatus&& from) noexcept
    : RobotStatus() {
    *this = ::std::move(from);
  }

  inline RobotStatus& operator=(const RobotStatus& from) {
    CopyFrom(from);
    return *this;
  }
  inline RobotStatus& operator=(RobotStatus&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const RobotStatus& default_instance() {
    return *internal_default_instance();
  }
  static inline const RobotStatus* internal_default_instance() {
    return reinterpret_cast<const RobotStatus*>(
               &_RobotStatus_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RobotStatus& a, RobotStatus& b) {
    a.Swap(&b);
  }
  inline void Swap(RobotStatus* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(RobotStatus* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline RobotStatus* New() const final {
    return new RobotStatus();
  }

  RobotStatus* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RobotStatus>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const RobotStatus& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const RobotStatus& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RobotStatus* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RobotStatus";
  }
  protected:
  explicit RobotStatus(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef RobotStatus_RobotState RobotState;
  static constexpr RobotState DISABLED =
    RobotStatus_RobotState_DISABLED;
  static constexpr RobotState TELEOP =
    RobotStatus_RobotState_TELEOP;
  static constexpr RobotState AUTONOMOUS =
    RobotStatus_RobotState_AUTONOMOUS;
  static constexpr RobotState TEST =
    RobotStatus_RobotState_TEST;
  static inline bool RobotState_IsValid(int value) {
    return RobotStatus_RobotState_IsValid(value);
  }
  static constexpr RobotState RobotState_MIN =
    RobotStatus_RobotState_RobotState_MIN;
  static constexpr RobotState RobotState_MAX =
    RobotStatus_RobotState_RobotState_MAX;
  static constexpr int RobotState_ARRAYSIZE =
    RobotStatus_RobotState_RobotState_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  RobotState_descriptor() {
    return RobotStatus_RobotState_descriptor();
  }
  template<typename T>
  static inline const std::string& RobotState_Name(T enum_t_value) {
    static_assert(::std::is_same<T, RobotState>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function RobotState_Name.");
    return RobotStatus_RobotState_Name(enum_t_value);
  }
  static inline bool RobotState_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      RobotState* value) {
    return RobotStatus_RobotState_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kRobotStateFieldNumber = 1,
  };
  // .RobotStatus.RobotState robot_state = 1;
  void clear_robot_state();
  ::RobotStatus_RobotState robot_state() const;
  void set_robot_state(::RobotStatus_RobotState value);
  private:
  ::RobotStatus_RobotState _internal_robot_state() const;
  void _internal_set_robot_state(::RobotStatus_RobotState value);
  public:

  // @@protoc_insertion_point(class_scope:RobotStatus)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int robot_state_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_RobotStatus_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RobotStatus

// .RobotStatus.RobotState robot_state = 1;
inline void RobotStatus::clear_robot_state() {
  robot_state_ = 0;
}
inline ::RobotStatus_RobotState RobotStatus::_internal_robot_state() const {
  return static_cast< ::RobotStatus_RobotState >(robot_state_);
}
inline ::RobotStatus_RobotState RobotStatus::robot_state() const {
  // @@protoc_insertion_point(field_get:RobotStatus.robot_state)
  return _internal_robot_state();
}
inline void RobotStatus::_internal_set_robot_state(::RobotStatus_RobotState value) {
  
  robot_state_ = value;
}
inline void RobotStatus::set_robot_state(::RobotStatus_RobotState value) {
  _internal_set_robot_state(value);
  // @@protoc_insertion_point(field_set:RobotStatus.robot_state)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::RobotStatus_RobotState> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::RobotStatus_RobotState>() {
  return ::RobotStatus_RobotState_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_RobotStatus_2eproto
