// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "short_motion.pb.h"

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

const ::google::protobuf::Descriptor* BatchShortMotionRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BatchShortMotionRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* BatchShortMotionResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BatchShortMotionResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_short_5fmotion_2eproto() {
  protobuf_AddDesc_short_5fmotion_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "short_motion.proto");
  GOOGLE_CHECK(file != NULL);
  BatchShortMotionRequest_descriptor_ = file->message_type(0);
  static const int BatchShortMotionRequest_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, short_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, is_desc_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, index_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, count_range_),
  };
  BatchShortMotionRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BatchShortMotionRequest_descriptor_,
      BatchShortMotionRequest::default_instance_,
      BatchShortMotionRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BatchShortMotionRequest));
  BatchShortMotionResponse_descriptor_ = file->message_type(1);
  static const int BatchShortMotionResponse_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionResponse, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionResponse, stk_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionResponse, total_cnt_),
  };
  BatchShortMotionResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BatchShortMotionResponse_descriptor_,
      BatchShortMotionResponse::default_instance_,
      BatchShortMotionResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BatchShortMotionResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BatchShortMotionResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_short_5fmotion_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BatchShortMotionRequest_descriptor_, &BatchShortMotionRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BatchShortMotionResponse_descriptor_, &BatchShortMotionResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_short_5fmotion_2eproto() {
  delete BatchShortMotionRequest::default_instance_;
  delete BatchShortMotionRequest_reflection_;
  delete BatchShortMotionResponse::default_instance_;
  delete BatchShortMotionResponse_reflection_;
}

void protobuf_AddDesc_short_5fmotion_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::QuoteProto::protobuf_AddDesc_ret_5fbase_2eproto();
  ::QuoteProto::protobuf_AddDesc_public_5fmessage_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022short_motion.proto\022\nQuoteProto\032\016ret_ba"
    "se.proto\032\024public_message.proto\"\205\001\n\027Batch"
    "ShortMotionRequest\0225\n\nshort_type\030\001 \002(\0162!"
    ".QuoteProto.ShortMotionClientType\022\017\n\007is_"
    "desc\030\002 \002(\010\022\r\n\005index\030\003 \002(\005\022\023\n\013count_range"
    "\030\004 \002(\005\"t\n\030BatchShortMotionResponse\022\034\n\003re"
    "t\030\001 \002(\0132\017.QuoteProto.Ret\022\'\n\003stk\030\002 \003(\0132\032."
    "QuoteProto.ShortMotionMsg\022\021\n\ttotal_cnt\030\003"
    " \001(\005", 324);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "short_motion.proto", &protobuf_RegisterTypes);
  BatchShortMotionRequest::default_instance_ = new BatchShortMotionRequest();
  BatchShortMotionResponse::default_instance_ = new BatchShortMotionResponse();
  BatchShortMotionRequest::default_instance_->InitAsDefaultInstance();
  BatchShortMotionResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_short_5fmotion_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_short_5fmotion_2eproto {
  StaticDescriptorInitializer_short_5fmotion_2eproto() {
    protobuf_AddDesc_short_5fmotion_2eproto();
  }
} static_descriptor_initializer_short_5fmotion_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int BatchShortMotionRequest::kShortTypeFieldNumber;
const int BatchShortMotionRequest::kIsDescFieldNumber;
const int BatchShortMotionRequest::kIndexFieldNumber;
const int BatchShortMotionRequest::kCountRangeFieldNumber;
#endif  // !_MSC_VER

BatchShortMotionRequest::BatchShortMotionRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BatchShortMotionRequest::InitAsDefaultInstance() {
}

BatchShortMotionRequest::BatchShortMotionRequest(const BatchShortMotionRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BatchShortMotionRequest::SharedCtor() {
  _cached_size_ = 0;
  short_type_ = 1;
  is_desc_ = false;
  index_ = 0;
  count_range_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BatchShortMotionRequest::~BatchShortMotionRequest() {
  SharedDtor();
}

void BatchShortMotionRequest::SharedDtor() {
  if (this != default_instance_) {
  }
}

void BatchShortMotionRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BatchShortMotionRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BatchShortMotionRequest_descriptor_;
}

const BatchShortMotionRequest& BatchShortMotionRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_short_5fmotion_2eproto();  return *default_instance_;
}

BatchShortMotionRequest* BatchShortMotionRequest::default_instance_ = NULL;

BatchShortMotionRequest* BatchShortMotionRequest::New() const {
  return new BatchShortMotionRequest;
}

void BatchShortMotionRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    short_type_ = 1;
    is_desc_ = false;
    index_ = 0;
    count_range_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BatchShortMotionRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .QuoteProto.ShortMotionClientType short_type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (QuoteProto::ShortMotionClientType_IsValid(value)) {
            set_short_type(static_cast< QuoteProto::ShortMotionClientType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_is_desc;
        break;
      }
      
      // required bool is_desc = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_is_desc:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &is_desc_)));
          set_has_is_desc();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_index;
        break;
      }
      
      // required int32 index = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_index:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &index_)));
          set_has_index();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_count_range;
        break;
      }
      
      // required int32 count_range = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_count_range:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &count_range_)));
          set_has_count_range();
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

void BatchShortMotionRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .QuoteProto.ShortMotionClientType short_type = 1;
  if (has_short_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->short_type(), output);
  }
  
  // required bool is_desc = 2;
  if (has_is_desc()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->is_desc(), output);
  }
  
  // required int32 index = 3;
  if (has_index()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->index(), output);
  }
  
  // required int32 count_range = 4;
  if (has_count_range()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->count_range(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BatchShortMotionRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .QuoteProto.ShortMotionClientType short_type = 1;
  if (has_short_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->short_type(), target);
  }
  
  // required bool is_desc = 2;
  if (has_is_desc()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->is_desc(), target);
  }
  
  // required int32 index = 3;
  if (has_index()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->index(), target);
  }
  
  // required int32 count_range = 4;
  if (has_count_range()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->count_range(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BatchShortMotionRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .QuoteProto.ShortMotionClientType short_type = 1;
    if (has_short_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->short_type());
    }
    
    // required bool is_desc = 2;
    if (has_is_desc()) {
      total_size += 1 + 1;
    }
    
    // required int32 index = 3;
    if (has_index()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->index());
    }
    
    // required int32 count_range = 4;
    if (has_count_range()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->count_range());
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

void BatchShortMotionRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BatchShortMotionRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BatchShortMotionRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BatchShortMotionRequest::MergeFrom(const BatchShortMotionRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_short_type()) {
      set_short_type(from.short_type());
    }
    if (from.has_is_desc()) {
      set_is_desc(from.is_desc());
    }
    if (from.has_index()) {
      set_index(from.index());
    }
    if (from.has_count_range()) {
      set_count_range(from.count_range());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BatchShortMotionRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BatchShortMotionRequest::CopyFrom(const BatchShortMotionRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BatchShortMotionRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;
  
  return true;
}

void BatchShortMotionRequest::Swap(BatchShortMotionRequest* other) {
  if (other != this) {
    std::swap(short_type_, other->short_type_);
    std::swap(is_desc_, other->is_desc_);
    std::swap(index_, other->index_);
    std::swap(count_range_, other->count_range_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BatchShortMotionRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BatchShortMotionRequest_descriptor_;
  metadata.reflection = BatchShortMotionRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int BatchShortMotionResponse::kRetFieldNumber;
const int BatchShortMotionResponse::kStkFieldNumber;
const int BatchShortMotionResponse::kTotalCntFieldNumber;
#endif  // !_MSC_VER

BatchShortMotionResponse::BatchShortMotionResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BatchShortMotionResponse::InitAsDefaultInstance() {
  ret_ = const_cast< ::QuoteProto::Ret*>(&::QuoteProto::Ret::default_instance());
}

BatchShortMotionResponse::BatchShortMotionResponse(const BatchShortMotionResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BatchShortMotionResponse::SharedCtor() {
  _cached_size_ = 0;
  ret_ = NULL;
  total_cnt_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BatchShortMotionResponse::~BatchShortMotionResponse() {
  SharedDtor();
}

void BatchShortMotionResponse::SharedDtor() {
  if (this != default_instance_) {
    delete ret_;
  }
}

void BatchShortMotionResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BatchShortMotionResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BatchShortMotionResponse_descriptor_;
}

const BatchShortMotionResponse& BatchShortMotionResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_short_5fmotion_2eproto();  return *default_instance_;
}

BatchShortMotionResponse* BatchShortMotionResponse::default_instance_ = NULL;

BatchShortMotionResponse* BatchShortMotionResponse::New() const {
  return new BatchShortMotionResponse;
}

void BatchShortMotionResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_ret()) {
      if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
    }
    total_cnt_ = 0;
  }
  stk_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BatchShortMotionResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .QuoteProto.Ret ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_ret()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_stk;
        break;
      }
      
      // repeated .QuoteProto.ShortMotionMsg stk = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_stk:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_stk()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_stk;
        if (input->ExpectTag(24)) goto parse_total_cnt;
        break;
      }
      
      // optional int32 total_cnt = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_total_cnt:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &total_cnt_)));
          set_has_total_cnt();
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

void BatchShortMotionResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .QuoteProto.Ret ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->ret(), output);
  }
  
  // repeated .QuoteProto.ShortMotionMsg stk = 2;
  for (int i = 0; i < this->stk_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->stk(i), output);
  }
  
  // optional int32 total_cnt = 3;
  if (has_total_cnt()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->total_cnt(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BatchShortMotionResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .QuoteProto.Ret ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->ret(), target);
  }
  
  // repeated .QuoteProto.ShortMotionMsg stk = 2;
  for (int i = 0; i < this->stk_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->stk(i), target);
  }
  
  // optional int32 total_cnt = 3;
  if (has_total_cnt()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->total_cnt(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BatchShortMotionResponse::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .QuoteProto.Ret ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->ret());
    }
    
    // optional int32 total_cnt = 3;
    if (has_total_cnt()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->total_cnt());
    }
    
  }
  // repeated .QuoteProto.ShortMotionMsg stk = 2;
  total_size += 1 * this->stk_size();
  for (int i = 0; i < this->stk_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->stk(i));
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

void BatchShortMotionResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BatchShortMotionResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BatchShortMotionResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BatchShortMotionResponse::MergeFrom(const BatchShortMotionResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  stk_.MergeFrom(from.stk_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      mutable_ret()->::QuoteProto::Ret::MergeFrom(from.ret());
    }
    if (from.has_total_cnt()) {
      set_total_cnt(from.total_cnt());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BatchShortMotionResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BatchShortMotionResponse::CopyFrom(const BatchShortMotionResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BatchShortMotionResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_ret()) {
    if (!this->ret().IsInitialized()) return false;
  }
  return true;
}

void BatchShortMotionResponse::Swap(BatchShortMotionResponse* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    stk_.Swap(&other->stk_);
    std::swap(total_cnt_, other->total_cnt_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BatchShortMotionResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BatchShortMotionResponse_descriptor_;
  metadata.reflection = BatchShortMotionResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

// @@protoc_insertion_point(global_scope)
