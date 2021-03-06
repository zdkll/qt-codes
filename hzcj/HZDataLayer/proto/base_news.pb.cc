// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "base_news.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace QuoteProto {

namespace {

const ::google::protobuf::Descriptor* BaseHead_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BaseHead_reflection_ = NULL;
const ::google::protobuf::Descriptor* BaseBody_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BaseBody_reflection_ = NULL;
const ::google::protobuf::Descriptor* BaseMsg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BaseMsg_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* EnumCmdType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_base_5fnews_2eproto() {
  protobuf_AddDesc_base_5fnews_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "base_news.proto");
  GOOGLE_CHECK(file != NULL);
  BaseHead_descriptor_ = file->message_type(0);
  static const int BaseHead_offsets_[7] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, req_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, pack_len_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, cmd_set_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, seq_set_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, uid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, version_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, safe_flag_),
  };
  BaseHead_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BaseHead_descriptor_,
      BaseHead::default_instance_,
      BaseHead_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseHead, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BaseHead));
  BaseBody_descriptor_ = file->message_type(1);
  static const int BaseBody_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseBody, msg_data_),
  };
  BaseBody_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BaseBody_descriptor_,
      BaseBody::default_instance_,
      BaseBody_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseBody, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseBody, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BaseBody));
  BaseMsg_descriptor_ = file->message_type(2);
  static const int BaseMsg_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseMsg, head_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseMsg, body_),
  };
  BaseMsg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BaseMsg_descriptor_,
      BaseMsg::default_instance_,
      BaseMsg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseMsg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseMsg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BaseMsg));
  EnumCmdType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_base_5fnews_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BaseHead_descriptor_, &BaseHead::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BaseBody_descriptor_, &BaseBody::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BaseMsg_descriptor_, &BaseMsg::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_base_5fnews_2eproto() {
  delete BaseHead::default_instance_;
  delete BaseHead_reflection_;
  delete BaseBody::default_instance_;
  delete BaseBody_reflection_;
  delete BaseMsg::default_instance_;
  delete BaseMsg_reflection_;
}

void protobuf_AddDesc_base_5fnews_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017base_news.proto\022\nQuoteProto\"\230\001\n\010BaseHe"
    "ad\022\016\n\006req_ID\030\001 \002(\004\022\020\n\010pack_len\030\002 \002(\003\022(\n\007"
    "cmd_set\030\003 \002(\0162\027.QuoteProto.EnumCmdType\022\017"
    "\n\007seq_set\030\004 \002(\003\022\013\n\003UID\030\005 \002(\005\022\017\n\007version\030"
    "\006 \002(\t\022\021\n\tsafe_flag\030\007 \001(\005\"\034\n\010BaseBody\022\020\n\010"
    "msg_data\030\001 \001(\014\"Q\n\007BaseMsg\022\"\n\004head\030\001 \002(\0132"
    "\024.QuoteProto.BaseHead\022\"\n\004body\030\002 \002(\0132\024.Qu"
    "oteProto.BaseBody*+\n\013EnumCmdType\022\017\n\013logi"
    "n_sever\020\001\022\013\n\007DEFAULT\020\002", 342);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "base_news.proto", &protobuf_RegisterTypes);
  BaseHead::default_instance_ = new BaseHead();
  BaseBody::default_instance_ = new BaseBody();
  BaseMsg::default_instance_ = new BaseMsg();
  BaseHead::default_instance_->InitAsDefaultInstance();
  BaseBody::default_instance_->InitAsDefaultInstance();
  BaseMsg::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_base_5fnews_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_base_5fnews_2eproto {
  StaticDescriptorInitializer_base_5fnews_2eproto() {
    protobuf_AddDesc_base_5fnews_2eproto();
  }
} static_descriptor_initializer_base_5fnews_2eproto_;

const ::google::protobuf::EnumDescriptor* EnumCmdType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return EnumCmdType_descriptor_;
}
bool EnumCmdType_IsValid(int value) {
  switch(value) {
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int BaseHead::kReqIDFieldNumber;
const int BaseHead::kPackLenFieldNumber;
const int BaseHead::kCmdSetFieldNumber;
const int BaseHead::kSeqSetFieldNumber;
const int BaseHead::kUIDFieldNumber;
const int BaseHead::kVersionFieldNumber;
const int BaseHead::kSafeFlagFieldNumber;
#endif  // !_MSC_VER

BaseHead::BaseHead()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BaseHead::InitAsDefaultInstance() {
}

BaseHead::BaseHead(const BaseHead& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BaseHead::SharedCtor() {
  _cached_size_ = 0;
  req_id_ = GOOGLE_ULONGLONG(0);
  pack_len_ = GOOGLE_LONGLONG(0);
  cmd_set_ = 1;
  seq_set_ = GOOGLE_LONGLONG(0);
  uid_ = 0;
  version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  safe_flag_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BaseHead::~BaseHead() {
  SharedDtor();
}

void BaseHead::SharedDtor() {
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    delete version_;
  }
  if (this != default_instance_) {
  }
}

void BaseHead::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BaseHead::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BaseHead_descriptor_;
}

const BaseHead& BaseHead::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_base_5fnews_2eproto();  return *default_instance_;
}

BaseHead* BaseHead::default_instance_ = NULL;

BaseHead* BaseHead::New() const {
  return new BaseHead;
}

void BaseHead::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    req_id_ = GOOGLE_ULONGLONG(0);
    pack_len_ = GOOGLE_LONGLONG(0);
    cmd_set_ = 1;
    seq_set_ = GOOGLE_LONGLONG(0);
    uid_ = 0;
    if (has_version()) {
      if (version_ != &::google::protobuf::internal::kEmptyString) {
        version_->clear();
      }
    }
    safe_flag_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BaseHead::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint64 req_ID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &req_id_)));
          set_has_req_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_pack_len;
        break;
      }
      
      // required int64 pack_len = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_pack_len:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &pack_len_)));
          set_has_pack_len();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_cmd_set;
        break;
      }
      
      // required .QuoteProto.EnumCmdType cmd_set = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_cmd_set:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (QuoteProto::EnumCmdType_IsValid(value)) {
            set_cmd_set(static_cast< QuoteProto::EnumCmdType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(3, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_seq_set;
        break;
      }
      
      // required int64 seq_set = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_seq_set:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &seq_set_)));
          set_has_seq_set();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_UID;
        break;
      }
      
      // required int32 UID = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_UID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &uid_)));
          set_has_uid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_version;
        break;
      }
      
      // required string version = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_version:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_version()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->version().data(), this->version().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_safe_flag;
        break;
      }
      
      // optional int32 safe_flag = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_safe_flag:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &safe_flag_)));
          set_has_safe_flag();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BaseHead::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint64 req_ID = 1;
  if (has_req_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->req_id(), output);
  }
  
  // required int64 pack_len = 2;
  if (has_pack_len()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->pack_len(), output);
  }
  
  // required .QuoteProto.EnumCmdType cmd_set = 3;
  if (has_cmd_set()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->cmd_set(), output);
  }
  
  // required int64 seq_set = 4;
  if (has_seq_set()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(4, this->seq_set(), output);
  }
  
  // required int32 UID = 5;
  if (has_uid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->uid(), output);
  }
  
  // required string version = 6;
  if (has_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->version().data(), this->version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->version(), output);
  }
  
  // optional int32 safe_flag = 7;
  if (has_safe_flag()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(7, this->safe_flag(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BaseHead::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint64 req_ID = 1;
  if (has_req_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(1, this->req_id(), target);
  }
  
  // required int64 pack_len = 2;
  if (has_pack_len()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->pack_len(), target);
  }
  
  // required .QuoteProto.EnumCmdType cmd_set = 3;
  if (has_cmd_set()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->cmd_set(), target);
  }
  
  // required int64 seq_set = 4;
  if (has_seq_set()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(4, this->seq_set(), target);
  }
  
  // required int32 UID = 5;
  if (has_uid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->uid(), target);
  }
  
  // required string version = 6;
  if (has_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->version().data(), this->version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->version(), target);
  }
  
  // optional int32 safe_flag = 7;
  if (has_safe_flag()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(7, this->safe_flag(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BaseHead::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint64 req_ID = 1;
    if (has_req_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->req_id());
    }
    
    // required int64 pack_len = 2;
    if (has_pack_len()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->pack_len());
    }
    
    // required .QuoteProto.EnumCmdType cmd_set = 3;
    if (has_cmd_set()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->cmd_set());
    }
    
    // required int64 seq_set = 4;
    if (has_seq_set()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->seq_set());
    }
    
    // required int32 UID = 5;
    if (has_uid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->uid());
    }
    
    // required string version = 6;
    if (has_version()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->version());
    }
    
    // optional int32 safe_flag = 7;
    if (has_safe_flag()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->safe_flag());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BaseHead::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BaseHead* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BaseHead*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BaseHead::MergeFrom(const BaseHead& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_req_id()) {
      set_req_id(from.req_id());
    }
    if (from.has_pack_len()) {
      set_pack_len(from.pack_len());
    }
    if (from.has_cmd_set()) {
      set_cmd_set(from.cmd_set());
    }
    if (from.has_seq_set()) {
      set_seq_set(from.seq_set());
    }
    if (from.has_uid()) {
      set_uid(from.uid());
    }
    if (from.has_version()) {
      set_version(from.version());
    }
    if (from.has_safe_flag()) {
      set_safe_flag(from.safe_flag());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BaseHead::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BaseHead::CopyFrom(const BaseHead& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BaseHead::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000003f) != 0x0000003f) return false;
  
  return true;
}

void BaseHead::Swap(BaseHead* other) {
  if (other != this) {
    std::swap(req_id_, other->req_id_);
    std::swap(pack_len_, other->pack_len_);
    std::swap(cmd_set_, other->cmd_set_);
    std::swap(seq_set_, other->seq_set_);
    std::swap(uid_, other->uid_);
    std::swap(version_, other->version_);
    std::swap(safe_flag_, other->safe_flag_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BaseHead::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BaseHead_descriptor_;
  metadata.reflection = BaseHead_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int BaseBody::kMsgDataFieldNumber;
#endif  // !_MSC_VER

BaseBody::BaseBody()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BaseBody::InitAsDefaultInstance() {
}

BaseBody::BaseBody(const BaseBody& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BaseBody::SharedCtor() {
  _cached_size_ = 0;
  msg_data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BaseBody::~BaseBody() {
  SharedDtor();
}

void BaseBody::SharedDtor() {
  if (msg_data_ != &::google::protobuf::internal::kEmptyString) {
    delete msg_data_;
  }
  if (this != default_instance_) {
  }
}

void BaseBody::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BaseBody::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BaseBody_descriptor_;
}

const BaseBody& BaseBody::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_base_5fnews_2eproto();  return *default_instance_;
}

BaseBody* BaseBody::default_instance_ = NULL;

BaseBody* BaseBody::New() const {
  return new BaseBody;
}

void BaseBody::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_msg_data()) {
      if (msg_data_ != &::google::protobuf::internal::kEmptyString) {
        msg_data_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BaseBody::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional bytes msg_data = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_msg_data()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BaseBody::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional bytes msg_data = 1;
  if (has_msg_data()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      1, this->msg_data(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BaseBody::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional bytes msg_data = 1;
  if (has_msg_data()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        1, this->msg_data(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BaseBody::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional bytes msg_data = 1;
    if (has_msg_data()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->msg_data());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BaseBody::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BaseBody* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BaseBody*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BaseBody::MergeFrom(const BaseBody& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_msg_data()) {
      set_msg_data(from.msg_data());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BaseBody::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BaseBody::CopyFrom(const BaseBody& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BaseBody::IsInitialized() const {
  
  return true;
}

void BaseBody::Swap(BaseBody* other) {
  if (other != this) {
    std::swap(msg_data_, other->msg_data_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BaseBody::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BaseBody_descriptor_;
  metadata.reflection = BaseBody_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int BaseMsg::kHeadFieldNumber;
const int BaseMsg::kBodyFieldNumber;
#endif  // !_MSC_VER

BaseMsg::BaseMsg()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BaseMsg::InitAsDefaultInstance() {
  head_ = const_cast< ::QuoteProto::BaseHead*>(&::QuoteProto::BaseHead::default_instance());
  body_ = const_cast< ::QuoteProto::BaseBody*>(&::QuoteProto::BaseBody::default_instance());
}

BaseMsg::BaseMsg(const BaseMsg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BaseMsg::SharedCtor() {
  _cached_size_ = 0;
  head_ = NULL;
  body_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BaseMsg::~BaseMsg() {
  SharedDtor();
}

void BaseMsg::SharedDtor() {
  if (this != default_instance_) {
    delete head_;
    delete body_;
  }
}

void BaseMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BaseMsg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BaseMsg_descriptor_;
}

const BaseMsg& BaseMsg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_base_5fnews_2eproto();  return *default_instance_;
}

BaseMsg* BaseMsg::default_instance_ = NULL;

BaseMsg* BaseMsg::New() const {
  return new BaseMsg;
}

void BaseMsg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_head()) {
      if (head_ != NULL) head_->::QuoteProto::BaseHead::Clear();
    }
    if (has_body()) {
      if (body_ != NULL) body_->::QuoteProto::BaseBody::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BaseMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .QuoteProto.BaseHead head = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_head()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_body;
        break;
      }
      
      // required .QuoteProto.BaseBody body = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_body:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_body()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BaseMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .QuoteProto.BaseHead head = 1;
  if (has_head()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->head(), output);
  }
  
  // required .QuoteProto.BaseBody body = 2;
  if (has_body()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->body(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BaseMsg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .QuoteProto.BaseHead head = 1;
  if (has_head()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->head(), target);
  }
  
  // required .QuoteProto.BaseBody body = 2;
  if (has_body()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->body(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BaseMsg::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .QuoteProto.BaseHead head = 1;
    if (has_head()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->head());
    }
    
    // required .QuoteProto.BaseBody body = 2;
    if (has_body()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->body());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BaseMsg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BaseMsg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BaseMsg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BaseMsg::MergeFrom(const BaseMsg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_head()) {
      mutable_head()->::QuoteProto::BaseHead::MergeFrom(from.head());
    }
    if (from.has_body()) {
      mutable_body()->::QuoteProto::BaseBody::MergeFrom(from.body());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BaseMsg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BaseMsg::CopyFrom(const BaseMsg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BaseMsg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  if (has_head()) {
    if (!this->head().IsInitialized()) return false;
  }
  return true;
}

void BaseMsg::Swap(BaseMsg* other) {
  if (other != this) {
    std::swap(head_, other->head_);
    std::swap(body_, other->body_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BaseMsg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BaseMsg_descriptor_;
  metadata.reflection = BaseMsg_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

// @@protoc_insertion_point(global_scope)
