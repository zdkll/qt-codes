// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: search.proto

#ifndef PROTOBUF_search_2eproto__INCLUDED
#define PROTOBUF_search_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "ret_base.pb.h"
// @@protoc_insertion_point(includes)

namespace QuoteProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_search_2eproto();
void protobuf_AssignDesc_search_2eproto();
void protobuf_ShutdownFile_search_2eproto();

class CodeTable;
class QueryCodeRequest;
class QueryCodeResponse;

enum SearchType {
  FuzzyQuery = 0,
  CodeQuery = 1,
  SampleQuery = 2,
  ChineseQuery = 4
};
bool SearchType_IsValid(int value);
const SearchType SearchType_MIN = FuzzyQuery;
const SearchType SearchType_MAX = ChineseQuery;
const int SearchType_ARRAYSIZE = SearchType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SearchType_descriptor();
inline const ::std::string& SearchType_Name(SearchType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SearchType_descriptor(), value);
}
inline bool SearchType_Parse(
    const ::std::string& name, SearchType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SearchType>(
    SearchType_descriptor(), name, value);
}
// ===================================================================

class CodeTable : public ::google::protobuf::Message {
 public:
  CodeTable();
  virtual ~CodeTable();
  
  CodeTable(const CodeTable& from);
  
  inline CodeTable& operator=(const CodeTable& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const CodeTable& default_instance();
  
  void Swap(CodeTable* other);
  
  // implements Message ----------------------------------------------
  
  CodeTable* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CodeTable& from);
  void MergeFrom(const CodeTable& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline const ::std::string& type() const;
  inline void set_type(const ::std::string& value);
  inline void set_type(const char* value);
  inline void set_type(const char* value, size_t size);
  inline ::std::string* mutable_type();
  inline ::std::string* release_type();
  
  // optional string code = 2;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 2;
  inline const ::std::string& code() const;
  inline void set_code(const ::std::string& value);
  inline void set_code(const char* value);
  inline void set_code(const char* value, size_t size);
  inline ::std::string* mutable_code();
  inline ::std::string* release_code();
  
  // optional string name = 3;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 3;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.CodeTable)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_code();
  inline void clear_has_code();
  inline void set_has_name();
  inline void clear_has_name();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* type_;
  ::std::string* code_;
  ::std::string* name_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_search_2eproto();
  friend void protobuf_AssignDesc_search_2eproto();
  friend void protobuf_ShutdownFile_search_2eproto();
  
  void InitAsDefaultInstance();
  static CodeTable* default_instance_;
};
// -------------------------------------------------------------------

class QueryCodeRequest : public ::google::protobuf::Message {
 public:
  QueryCodeRequest();
  virtual ~QueryCodeRequest();
  
  QueryCodeRequest(const QueryCodeRequest& from);
  
  inline QueryCodeRequest& operator=(const QueryCodeRequest& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const QueryCodeRequest& default_instance();
  
  void Swap(QueryCodeRequest* other);
  
  // implements Message ----------------------------------------------
  
  QueryCodeRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const QueryCodeRequest& from);
  void MergeFrom(const QueryCodeRequest& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .QuoteProto.SearchType type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline QuoteProto::SearchType type() const;
  inline void set_type(QuoteProto::SearchType value);
  
  // required string search_word = 2;
  inline bool has_search_word() const;
  inline void clear_search_word();
  static const int kSearchWordFieldNumber = 2;
  inline const ::std::string& search_word() const;
  inline void set_search_word(const ::std::string& value);
  inline void set_search_word(const char* value);
  inline void set_search_word(const char* value, size_t size);
  inline ::std::string* mutable_search_word();
  inline ::std::string* release_search_word();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.QueryCodeRequest)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_search_word();
  inline void clear_has_search_word();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* search_word_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_search_2eproto();
  friend void protobuf_AssignDesc_search_2eproto();
  friend void protobuf_ShutdownFile_search_2eproto();
  
  void InitAsDefaultInstance();
  static QueryCodeRequest* default_instance_;
};
// -------------------------------------------------------------------

class QueryCodeResponse : public ::google::protobuf::Message {
 public:
  QueryCodeResponse();
  virtual ~QueryCodeResponse();
  
  QueryCodeResponse(const QueryCodeResponse& from);
  
  inline QueryCodeResponse& operator=(const QueryCodeResponse& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const QueryCodeResponse& default_instance();
  
  void Swap(QueryCodeResponse* other);
  
  // implements Message ----------------------------------------------
  
  QueryCodeResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const QueryCodeResponse& from);
  void MergeFrom(const QueryCodeResponse& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .QuoteProto.Ret ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline const ::QuoteProto::Ret& ret() const;
  inline ::QuoteProto::Ret* mutable_ret();
  inline ::QuoteProto::Ret* release_ret();
  
  // repeated .QuoteProto.CodeTable codes_table = 3;
  inline int codes_table_size() const;
  inline void clear_codes_table();
  static const int kCodesTableFieldNumber = 3;
  inline const ::QuoteProto::CodeTable& codes_table(int index) const;
  inline ::QuoteProto::CodeTable* mutable_codes_table(int index);
  inline ::QuoteProto::CodeTable* add_codes_table();
  inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::CodeTable >&
      codes_table() const;
  inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::CodeTable >*
      mutable_codes_table();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.QueryCodeResponse)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::QuoteProto::Ret* ret_;
  ::google::protobuf::RepeatedPtrField< ::QuoteProto::CodeTable > codes_table_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_search_2eproto();
  friend void protobuf_AssignDesc_search_2eproto();
  friend void protobuf_ShutdownFile_search_2eproto();
  
  void InitAsDefaultInstance();
  static QueryCodeResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// CodeTable

// optional string type = 1;
inline bool CodeTable::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CodeTable::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CodeTable::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CodeTable::clear_type() {
  if (type_ != &::google::protobuf::internal::kEmptyString) {
    type_->clear();
  }
  clear_has_type();
}
inline const ::std::string& CodeTable::type() const {
  return *type_;
}
inline void CodeTable::set_type(const ::std::string& value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void CodeTable::set_type(const char* value) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(value);
}
inline void CodeTable::set_type(const char* value, size_t size) {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  type_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CodeTable::mutable_type() {
  set_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    type_ = new ::std::string;
  }
  return type_;
}
inline ::std::string* CodeTable::release_type() {
  clear_has_type();
  if (type_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = type_;
    type_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string code = 2;
inline bool CodeTable::has_code() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CodeTable::set_has_code() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CodeTable::clear_has_code() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CodeTable::clear_code() {
  if (code_ != &::google::protobuf::internal::kEmptyString) {
    code_->clear();
  }
  clear_has_code();
}
inline const ::std::string& CodeTable::code() const {
  return *code_;
}
inline void CodeTable::set_code(const ::std::string& value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(value);
}
inline void CodeTable::set_code(const char* value) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(value);
}
inline void CodeTable::set_code(const char* value, size_t size) {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  code_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CodeTable::mutable_code() {
  set_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    code_ = new ::std::string;
  }
  return code_;
}
inline ::std::string* CodeTable::release_code() {
  clear_has_code();
  if (code_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = code_;
    code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string name = 3;
inline bool CodeTable::has_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void CodeTable::set_has_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void CodeTable::clear_has_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void CodeTable::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& CodeTable::name() const {
  return *name_;
}
inline void CodeTable::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void CodeTable::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void CodeTable::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CodeTable::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* CodeTable::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// QueryCodeRequest

// required .QuoteProto.SearchType type = 1;
inline bool QueryCodeRequest::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void QueryCodeRequest::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void QueryCodeRequest::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void QueryCodeRequest::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline QuoteProto::SearchType QueryCodeRequest::type() const {
  return static_cast< QuoteProto::SearchType >(type_);
}
inline void QueryCodeRequest::set_type(QuoteProto::SearchType value) {
  GOOGLE_DCHECK(QuoteProto::SearchType_IsValid(value));
  set_has_type();
  type_ = value;
}

// required string search_word = 2;
inline bool QueryCodeRequest::has_search_word() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void QueryCodeRequest::set_has_search_word() {
  _has_bits_[0] |= 0x00000002u;
}
inline void QueryCodeRequest::clear_has_search_word() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void QueryCodeRequest::clear_search_word() {
  if (search_word_ != &::google::protobuf::internal::kEmptyString) {
    search_word_->clear();
  }
  clear_has_search_word();
}
inline const ::std::string& QueryCodeRequest::search_word() const {
  return *search_word_;
}
inline void QueryCodeRequest::set_search_word(const ::std::string& value) {
  set_has_search_word();
  if (search_word_ == &::google::protobuf::internal::kEmptyString) {
    search_word_ = new ::std::string;
  }
  search_word_->assign(value);
}
inline void QueryCodeRequest::set_search_word(const char* value) {
  set_has_search_word();
  if (search_word_ == &::google::protobuf::internal::kEmptyString) {
    search_word_ = new ::std::string;
  }
  search_word_->assign(value);
}
inline void QueryCodeRequest::set_search_word(const char* value, size_t size) {
  set_has_search_word();
  if (search_word_ == &::google::protobuf::internal::kEmptyString) {
    search_word_ = new ::std::string;
  }
  search_word_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* QueryCodeRequest::mutable_search_word() {
  set_has_search_word();
  if (search_word_ == &::google::protobuf::internal::kEmptyString) {
    search_word_ = new ::std::string;
  }
  return search_word_;
}
inline ::std::string* QueryCodeRequest::release_search_word() {
  clear_has_search_word();
  if (search_word_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = search_word_;
    search_word_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// QueryCodeResponse

// required .QuoteProto.Ret ret = 1;
inline bool QueryCodeResponse::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void QueryCodeResponse::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void QueryCodeResponse::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void QueryCodeResponse::clear_ret() {
  if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
  clear_has_ret();
}
inline const ::QuoteProto::Ret& QueryCodeResponse::ret() const {
  return ret_ != NULL ? *ret_ : *default_instance_->ret_;
}
inline ::QuoteProto::Ret* QueryCodeResponse::mutable_ret() {
  set_has_ret();
  if (ret_ == NULL) ret_ = new ::QuoteProto::Ret;
  return ret_;
}
inline ::QuoteProto::Ret* QueryCodeResponse::release_ret() {
  clear_has_ret();
  ::QuoteProto::Ret* temp = ret_;
  ret_ = NULL;
  return temp;
}

// repeated .QuoteProto.CodeTable codes_table = 3;
inline int QueryCodeResponse::codes_table_size() const {
  return codes_table_.size();
}
inline void QueryCodeResponse::clear_codes_table() {
  codes_table_.Clear();
}
inline const ::QuoteProto::CodeTable& QueryCodeResponse::codes_table(int index) const {
  return codes_table_.Get(index);
}
inline ::QuoteProto::CodeTable* QueryCodeResponse::mutable_codes_table(int index) {
  return codes_table_.Mutable(index);
}
inline ::QuoteProto::CodeTable* QueryCodeResponse::add_codes_table() {
  return codes_table_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::CodeTable >&
QueryCodeResponse::codes_table() const {
  return codes_table_;
}
inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::CodeTable >*
QueryCodeResponse::mutable_codes_table() {
  return &codes_table_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< QuoteProto::SearchType>() {
  return QuoteProto::SearchType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_search_2eproto__INCLUDED
