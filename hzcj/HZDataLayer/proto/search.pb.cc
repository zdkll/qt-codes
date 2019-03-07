// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "search.pb.h"

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

const ::google::protobuf::Descriptor* CodeTable_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CodeTable_reflection_ = NULL;
const ::google::protobuf::Descriptor* QueryCodeRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  QueryCodeRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* QueryCodeResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  QueryCodeResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* SearchType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_search_2eproto() {
  protobuf_AddDesc_search_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "search.proto");
  GOOGLE_CHECK(file != NULL);
  CodeTable_descriptor_ = file->message_type(0);
  static const int CodeTable_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CodeTable, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CodeTable, code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CodeTable, name_),
  };
  CodeTable_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CodeTable_descriptor_,
      CodeTable::default_instance_,
      CodeTable_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CodeTable, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CodeTable, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CodeTable));
  QueryCodeRequest_descriptor_ = file->message_type(1);
  static const int QueryCodeRequest_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeRequest, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeRequest, search_word_),
  };
  QueryCodeRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      QueryCodeRequest_descriptor_,
      QueryCodeRequest::default_instance_,
      QueryCodeRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(QueryCodeRequest));
  QueryCodeResponse_descriptor_ = file->message_type(2);
  static const int QueryCodeResponse_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeResponse, ret_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeResponse, codes_table_),
  };
  QueryCodeResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      QueryCodeResponse_descriptor_,
      QueryCodeResponse::default_instance_,
      QueryCodeResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(QueryCodeResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(QueryCodeResponse));
  SearchType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_search_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CodeTable_descriptor_, &CodeTable::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    QueryCodeRequest_descriptor_, &QueryCodeRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    QueryCodeResponse_descriptor_, &QueryCodeResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_search_2eproto() {
  delete CodeTable::default_instance_;
  delete CodeTable_reflection_;
  delete QueryCodeRequest::default_instance_;
  delete QueryCodeRequest_reflection_;
  delete QueryCodeResponse::default_instance_;
  delete QueryCodeResponse_reflection_;
}

void protobuf_AddDesc_search_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::QuoteProto::protobuf_AddDesc_ret_5fbase_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014search.proto\022\nQuoteProto\032\016ret_base.pro"
    "to\"5\n\tCodeTable\022\014\n\004type\030\001 \001(\t\022\014\n\004code\030\002 "
    "\001(\t\022\014\n\004name\030\003 \001(\t\"M\n\020QueryCodeRequest\022$\n"
    "\004type\030\001 \002(\0162\026.QuoteProto.SearchType\022\023\n\013s"
    "earch_word\030\002 \002(\t\"]\n\021QueryCodeResponse\022\034\n"
    "\003ret\030\001 \002(\0132\017.QuoteProto.Ret\022*\n\013codes_tab"
    "le\030\003 \003(\0132\025.QuoteProto.CodeTable*N\n\nSearc"
    "hType\022\016\n\nFuzzyQuery\020\000\022\r\n\tCodeQuery\020\001\022\017\n\013"
    "SampleQuery\020\002\022\020\n\014ChineseQuery\020\004", 351);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "search.proto", &protobuf_RegisterTypes);
  CodeTable::default_instance_ = new CodeTable();
  QueryCodeRequest::default_instance_ = new QueryCodeRequest();
  QueryCodeResponse::default_instance_ = new QueryCodeResponse();
  CodeTable::default_instance_->InitAsDefaultInstance();
  QueryCodeRequest::default_instance_->InitAsDefaultInstance();
  QueryCodeResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_search_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_search_2eproto {
  StaticDescriptorInitializer_search_2eproto() {
    protobuf_AddDesc_search_2eproto();
  }
} static_descriptor_initializer_search_2eproto_;

const ::google::protobuf::EnumDescriptor* SearchType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SearchType_descriptor_;
}
bool SearchType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int CodeTable::kTypeFieldNumber;
const int CodeTable::kCodeFieldNumber;
const int CodeTable::kNameFieldNumber;
#endif  // !_MSC_VER

CodeTable::CodeTable()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CodeTable::InitAsDefaultInstance() {
}

CodeTable::CodeTable(const CodeTable& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CodeTable::SharedCtor() {
  _cached_size_ = 0;
  type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CodeTable::~CodeTable() {
  SharedDtor();
}

void CodeTable::SharedDtor() {
  if (type_ != &::google::protobuf::internal::kEmptyString) {
    delete type_;
  }
  if (code_ != &::google::protobuf::internal::kEmptyString) {
    delete code_;
  }
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (this != default_instance_) {
  }
}

void CodeTable::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CodeTable::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CodeTable_descriptor_;
}

const CodeTable& CodeTable::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_search_2eproto();  return *default_instance_;
}

CodeTable* CodeTable::default_instance_ = NULL;

CodeTable* CodeTable::New() const {
  return new CodeTable;
}

void CodeTable::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_type()) {
      if (type_ != &::google::protobuf::internal::kEmptyString) {
        type_->clear();
      }
    }
    if (has_code()) {
      if (code_ != &::google::protobuf::internal::kEmptyString) {
        code_->clear();
      }
    }
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::kEmptyString) {
        name_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CodeTable::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_type()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->type().data(), this->type().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_code;
        break;
      }
      
      // optional string code = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_code:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_code()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->code().data(), this->code().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_name;
        break;
      }
      
      // optional string name = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void CodeTable::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->type().data(), this->type().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->type(), output);
  }
  
  // optional string code = 2;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->code(), output);
  }
  
  // optional string name = 3;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->name(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CodeTable::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->type().data(), this->type().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->type(), target);
  }
  
  // optional string code = 2;
  if (has_code()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->code().data(), this->code().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->code(), target);
  }
  
  // optional string name = 3;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->name(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CodeTable::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->type());
    }
    
    // optional string code = 2;
    if (has_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->code());
    }
    
    // optional string name = 3;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
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

void CodeTable::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CodeTable* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CodeTable*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CodeTable::MergeFrom(const CodeTable& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_code()) {
      set_code(from.code());
    }
    if (from.has_name()) {
      set_name(from.name());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CodeTable::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CodeTable::CopyFrom(const CodeTable& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CodeTable::IsInitialized() const {
  
  return true;
}

void CodeTable::Swap(CodeTable* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(code_, other->code_);
    std::swap(name_, other->name_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CodeTable::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CodeTable_descriptor_;
  metadata.reflection = CodeTable_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int QueryCodeRequest::kTypeFieldNumber;
const int QueryCodeRequest::kSearchWordFieldNumber;
#endif  // !_MSC_VER

QueryCodeRequest::QueryCodeRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void QueryCodeRequest::InitAsDefaultInstance() {
}

QueryCodeRequest::QueryCodeRequest(const QueryCodeRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void QueryCodeRequest::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  search_word_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

QueryCodeRequest::~QueryCodeRequest() {
  SharedDtor();
}

void QueryCodeRequest::SharedDtor() {
  if (search_word_ != &::google::protobuf::internal::kEmptyString) {
    delete search_word_;
  }
  if (this != default_instance_) {
  }
}

void QueryCodeRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* QueryCodeRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return QueryCodeRequest_descriptor_;
}

const QueryCodeRequest& QueryCodeRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_search_2eproto();  return *default_instance_;
}

QueryCodeRequest* QueryCodeRequest::default_instance_ = NULL;

QueryCodeRequest* QueryCodeRequest::New() const {
  return new QueryCodeRequest;
}

void QueryCodeRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
    if (has_search_word()) {
      if (search_word_ != &::google::protobuf::internal::kEmptyString) {
        search_word_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool QueryCodeRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .QuoteProto.SearchType type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (QuoteProto::SearchType_IsValid(value)) {
            set_type(static_cast< QuoteProto::SearchType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_search_word;
        break;
      }
      
      // required string search_word = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_search_word:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_search_word()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->search_word().data(), this->search_word().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void QueryCodeRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .QuoteProto.SearchType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }
  
  // required string search_word = 2;
  if (has_search_word()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->search_word().data(), this->search_word().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->search_word(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* QueryCodeRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .QuoteProto.SearchType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }
  
  // required string search_word = 2;
  if (has_search_word()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->search_word().data(), this->search_word().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->search_word(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int QueryCodeRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .QuoteProto.SearchType type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }
    
    // required string search_word = 2;
    if (has_search_word()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->search_word());
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

void QueryCodeRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const QueryCodeRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const QueryCodeRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void QueryCodeRequest::MergeFrom(const QueryCodeRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_search_word()) {
      set_search_word(from.search_word());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void QueryCodeRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void QueryCodeRequest::CopyFrom(const QueryCodeRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool QueryCodeRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void QueryCodeRequest::Swap(QueryCodeRequest* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(search_word_, other->search_word_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata QueryCodeRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = QueryCodeRequest_descriptor_;
  metadata.reflection = QueryCodeRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int QueryCodeResponse::kRetFieldNumber;
const int QueryCodeResponse::kCodesTableFieldNumber;
#endif  // !_MSC_VER

QueryCodeResponse::QueryCodeResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void QueryCodeResponse::InitAsDefaultInstance() {
  ret_ = const_cast< ::QuoteProto::Ret*>(&::QuoteProto::Ret::default_instance());
}

QueryCodeResponse::QueryCodeResponse(const QueryCodeResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void QueryCodeResponse::SharedCtor() {
  _cached_size_ = 0;
  ret_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

QueryCodeResponse::~QueryCodeResponse() {
  SharedDtor();
}

void QueryCodeResponse::SharedDtor() {
  if (this != default_instance_) {
    delete ret_;
  }
}

void QueryCodeResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* QueryCodeResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return QueryCodeResponse_descriptor_;
}

const QueryCodeResponse& QueryCodeResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_search_2eproto();  return *default_instance_;
}

QueryCodeResponse* QueryCodeResponse::default_instance_ = NULL;

QueryCodeResponse* QueryCodeResponse::New() const {
  return new QueryCodeResponse;
}

void QueryCodeResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_ret()) {
      if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
    }
  }
  codes_table_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool QueryCodeResponse::MergePartialFromCodedStream(
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
        if (input->ExpectTag(26)) goto parse_codes_table;
        break;
      }
      
      // repeated .QuoteProto.CodeTable codes_table = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_codes_table:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_codes_table()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_codes_table;
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

void QueryCodeResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .QuoteProto.Ret ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->ret(), output);
  }
  
  // repeated .QuoteProto.CodeTable codes_table = 3;
  for (int i = 0; i < this->codes_table_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->codes_table(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* QueryCodeResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .QuoteProto.Ret ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->ret(), target);
  }
  
  // repeated .QuoteProto.CodeTable codes_table = 3;
  for (int i = 0; i < this->codes_table_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->codes_table(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int QueryCodeResponse::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .QuoteProto.Ret ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->ret());
    }
    
  }
  // repeated .QuoteProto.CodeTable codes_table = 3;
  total_size += 1 * this->codes_table_size();
  for (int i = 0; i < this->codes_table_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->codes_table(i));
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

void QueryCodeResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const QueryCodeResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const QueryCodeResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void QueryCodeResponse::MergeFrom(const QueryCodeResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  codes_table_.MergeFrom(from.codes_table_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      mutable_ret()->::QuoteProto::Ret::MergeFrom(from.ret());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void QueryCodeResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void QueryCodeResponse::CopyFrom(const QueryCodeResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool QueryCodeResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_ret()) {
    if (!this->ret().IsInitialized()) return false;
  }
  return true;
}

void QueryCodeResponse::Swap(QueryCodeResponse* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    codes_table_.Swap(&other->codes_table_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata QueryCodeResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = QueryCodeResponse_descriptor_;
  metadata.reflection = QueryCodeResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

// @@protoc_insertion_point(global_scope)