// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: base_news.proto

#ifndef PROTOBUF_base_5fnews_2eproto__INCLUDED
#define PROTOBUF_base_5fnews_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

namespace QuoteProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_base_5fnews_2eproto();
void protobuf_AssignDesc_base_5fnews_2eproto();
void protobuf_ShutdownFile_base_5fnews_2eproto();

class BaseHead;
class BaseBody;
class BaseMsg;

enum EnumCmdType {
  login_sever = 1,
  DEFAULT = 2
};
bool EnumCmdType_IsValid(int value);
const EnumCmdType EnumCmdType_MIN = login_sever;
const EnumCmdType EnumCmdType_MAX = DEFAULT;
const int EnumCmdType_ARRAYSIZE = EnumCmdType_MAX + 1;

const ::google::protobuf::EnumDescriptor* EnumCmdType_descriptor();
inline const ::std::string& EnumCmdType_Name(EnumCmdType value) {
  return ::google::protobuf::internal::NameOfEnum(
    EnumCmdType_descriptor(), value);
}
inline bool EnumCmdType_Parse(
    const ::std::string& name, EnumCmdType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EnumCmdType>(
    EnumCmdType_descriptor(), name, value);
}
// ===================================================================

class BaseHead : public ::google::protobuf::Message {
 public:
  BaseHead();
  virtual ~BaseHead();
  
  BaseHead(const BaseHead& from);
  
  inline BaseHead& operator=(const BaseHead& from) {
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
  static const BaseHead& default_instance();
  
  void Swap(BaseHead* other);
  
  // implements Message ----------------------------------------------
  
  BaseHead* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BaseHead& from);
  void MergeFrom(const BaseHead& from);
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
  
  // required uint64 req_ID = 1;
  inline bool has_req_id() const;
  inline void clear_req_id();
  static const int kReqIDFieldNumber = 1;
  inline ::google::protobuf::uint64 req_id() const;
  inline void set_req_id(::google::protobuf::uint64 value);
  
  // required int64 pack_len = 2;
  inline bool has_pack_len() const;
  inline void clear_pack_len();
  static const int kPackLenFieldNumber = 2;
  inline ::google::protobuf::int64 pack_len() const;
  inline void set_pack_len(::google::protobuf::int64 value);
  
  // required .QuoteProto.EnumCmdType cmd_set = 3;
  inline bool has_cmd_set() const;
  inline void clear_cmd_set();
  static const int kCmdSetFieldNumber = 3;
  inline QuoteProto::EnumCmdType cmd_set() const;
  inline void set_cmd_set(QuoteProto::EnumCmdType value);
  
  // required int64 seq_set = 4;
  inline bool has_seq_set() const;
  inline void clear_seq_set();
  static const int kSeqSetFieldNumber = 4;
  inline ::google::protobuf::int64 seq_set() const;
  inline void set_seq_set(::google::protobuf::int64 value);
  
  // required int32 UID = 5;
  inline bool has_uid() const;
  inline void clear_uid();
  static const int kUIDFieldNumber = 5;
  inline ::google::protobuf::int32 uid() const;
  inline void set_uid(::google::protobuf::int32 value);
  
  // required string version = 6;
  inline bool has_version() const;
  inline void clear_version();
  static const int kVersionFieldNumber = 6;
  inline const ::std::string& version() const;
  inline void set_version(const ::std::string& value);
  inline void set_version(const char* value);
  inline void set_version(const char* value, size_t size);
  inline ::std::string* mutable_version();
  inline ::std::string* release_version();
  
  // optional int32 safe_flag = 7;
  inline bool has_safe_flag() const;
  inline void clear_safe_flag();
  static const int kSafeFlagFieldNumber = 7;
  inline ::google::protobuf::int32 safe_flag() const;
  inline void set_safe_flag(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.BaseHead)
 private:
  inline void set_has_req_id();
  inline void clear_has_req_id();
  inline void set_has_pack_len();
  inline void clear_has_pack_len();
  inline void set_has_cmd_set();
  inline void clear_has_cmd_set();
  inline void set_has_seq_set();
  inline void clear_has_seq_set();
  inline void set_has_uid();
  inline void clear_has_uid();
  inline void set_has_version();
  inline void clear_has_version();
  inline void set_has_safe_flag();
  inline void clear_has_safe_flag();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint64 req_id_;
  ::google::protobuf::int64 pack_len_;
  ::google::protobuf::int64 seq_set_;
  int cmd_set_;
  ::google::protobuf::int32 uid_;
  ::std::string* version_;
  ::google::protobuf::int32 safe_flag_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];
  
  friend void  protobuf_AddDesc_base_5fnews_2eproto();
  friend void protobuf_AssignDesc_base_5fnews_2eproto();
  friend void protobuf_ShutdownFile_base_5fnews_2eproto();
  
  void InitAsDefaultInstance();
  static BaseHead* default_instance_;
};
// -------------------------------------------------------------------

class BaseBody : public ::google::protobuf::Message {
 public:
  BaseBody();
  virtual ~BaseBody();
  
  BaseBody(const BaseBody& from);
  
  inline BaseBody& operator=(const BaseBody& from) {
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
  static const BaseBody& default_instance();
  
  void Swap(BaseBody* other);
  
  // implements Message ----------------------------------------------
  
  BaseBody* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BaseBody& from);
  void MergeFrom(const BaseBody& from);
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
  
  // optional bytes msg_data = 1;
  inline bool has_msg_data() const;
  inline void clear_msg_data();
  static const int kMsgDataFieldNumber = 1;
  inline const ::std::string& msg_data() const;
  inline void set_msg_data(const ::std::string& value);
  inline void set_msg_data(const char* value);
  inline void set_msg_data(const void* value, size_t size);
  inline ::std::string* mutable_msg_data();
  inline ::std::string* release_msg_data();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.BaseBody)
 private:
  inline void set_has_msg_data();
  inline void clear_has_msg_data();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* msg_data_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_base_5fnews_2eproto();
  friend void protobuf_AssignDesc_base_5fnews_2eproto();
  friend void protobuf_ShutdownFile_base_5fnews_2eproto();
  
  void InitAsDefaultInstance();
  static BaseBody* default_instance_;
};
// -------------------------------------------------------------------

class BaseMsg : public ::google::protobuf::Message {
 public:
  BaseMsg();
  virtual ~BaseMsg();
  
  BaseMsg(const BaseMsg& from);
  
  inline BaseMsg& operator=(const BaseMsg& from) {
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
  static const BaseMsg& default_instance();
  
  void Swap(BaseMsg* other);
  
  // implements Message ----------------------------------------------
  
  BaseMsg* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BaseMsg& from);
  void MergeFrom(const BaseMsg& from);
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
  
  // required .QuoteProto.BaseHead head = 1;
  inline bool has_head() const;
  inline void clear_head();
  static const int kHeadFieldNumber = 1;
  inline const ::QuoteProto::BaseHead& head() const;
  inline ::QuoteProto::BaseHead* mutable_head();
  inline ::QuoteProto::BaseHead* release_head();
  
  // required .QuoteProto.BaseBody body = 2;
  inline bool has_body() const;
  inline void clear_body();
  static const int kBodyFieldNumber = 2;
  inline const ::QuoteProto::BaseBody& body() const;
  inline ::QuoteProto::BaseBody* mutable_body();
  inline ::QuoteProto::BaseBody* release_body();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.BaseMsg)
 private:
  inline void set_has_head();
  inline void clear_has_head();
  inline void set_has_body();
  inline void clear_has_body();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::QuoteProto::BaseHead* head_;
  ::QuoteProto::BaseBody* body_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_base_5fnews_2eproto();
  friend void protobuf_AssignDesc_base_5fnews_2eproto();
  friend void protobuf_ShutdownFile_base_5fnews_2eproto();
  
  void InitAsDefaultInstance();
  static BaseMsg* default_instance_;
};
// ===================================================================


// ===================================================================

// BaseHead

// required uint64 req_ID = 1;
inline bool BaseHead::has_req_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BaseHead::set_has_req_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BaseHead::clear_has_req_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BaseHead::clear_req_id() {
  req_id_ = GOOGLE_ULONGLONG(0);
  clear_has_req_id();
}
inline ::google::protobuf::uint64 BaseHead::req_id() const {
  return req_id_;
}
inline void BaseHead::set_req_id(::google::protobuf::uint64 value) {
  set_has_req_id();
  req_id_ = value;
}

// required int64 pack_len = 2;
inline bool BaseHead::has_pack_len() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void BaseHead::set_has_pack_len() {
  _has_bits_[0] |= 0x00000002u;
}
inline void BaseHead::clear_has_pack_len() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void BaseHead::clear_pack_len() {
  pack_len_ = GOOGLE_LONGLONG(0);
  clear_has_pack_len();
}
inline ::google::protobuf::int64 BaseHead::pack_len() const {
  return pack_len_;
}
inline void BaseHead::set_pack_len(::google::protobuf::int64 value) {
  set_has_pack_len();
  pack_len_ = value;
}

// required .QuoteProto.EnumCmdType cmd_set = 3;
inline bool BaseHead::has_cmd_set() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void BaseHead::set_has_cmd_set() {
  _has_bits_[0] |= 0x00000004u;
}
inline void BaseHead::clear_has_cmd_set() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void BaseHead::clear_cmd_set() {
  cmd_set_ = 1;
  clear_has_cmd_set();
}
inline QuoteProto::EnumCmdType BaseHead::cmd_set() const {
  return static_cast< QuoteProto::EnumCmdType >(cmd_set_);
}
inline void BaseHead::set_cmd_set(QuoteProto::EnumCmdType value) {
  GOOGLE_DCHECK(QuoteProto::EnumCmdType_IsValid(value));
  set_has_cmd_set();
  cmd_set_ = value;
}

// required int64 seq_set = 4;
inline bool BaseHead::has_seq_set() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void BaseHead::set_has_seq_set() {
  _has_bits_[0] |= 0x00000008u;
}
inline void BaseHead::clear_has_seq_set() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void BaseHead::clear_seq_set() {
  seq_set_ = GOOGLE_LONGLONG(0);
  clear_has_seq_set();
}
inline ::google::protobuf::int64 BaseHead::seq_set() const {
  return seq_set_;
}
inline void BaseHead::set_seq_set(::google::protobuf::int64 value) {
  set_has_seq_set();
  seq_set_ = value;
}

// required int32 UID = 5;
inline bool BaseHead::has_uid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void BaseHead::set_has_uid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void BaseHead::clear_has_uid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void BaseHead::clear_uid() {
  uid_ = 0;
  clear_has_uid();
}
inline ::google::protobuf::int32 BaseHead::uid() const {
  return uid_;
}
inline void BaseHead::set_uid(::google::protobuf::int32 value) {
  set_has_uid();
  uid_ = value;
}

// required string version = 6;
inline bool BaseHead::has_version() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void BaseHead::set_has_version() {
  _has_bits_[0] |= 0x00000020u;
}
inline void BaseHead::clear_has_version() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void BaseHead::clear_version() {
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    version_->clear();
  }
  clear_has_version();
}
inline const ::std::string& BaseHead::version() const {
  return *version_;
}
inline void BaseHead::set_version(const ::std::string& value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void BaseHead::set_version(const char* value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void BaseHead::set_version(const char* value, size_t size) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BaseHead::mutable_version() {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  return version_;
}
inline ::std::string* BaseHead::release_version() {
  clear_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = version_;
    version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 safe_flag = 7;
inline bool BaseHead::has_safe_flag() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void BaseHead::set_has_safe_flag() {
  _has_bits_[0] |= 0x00000040u;
}
inline void BaseHead::clear_has_safe_flag() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void BaseHead::clear_safe_flag() {
  safe_flag_ = 0;
  clear_has_safe_flag();
}
inline ::google::protobuf::int32 BaseHead::safe_flag() const {
  return safe_flag_;
}
inline void BaseHead::set_safe_flag(::google::protobuf::int32 value) {
  set_has_safe_flag();
  safe_flag_ = value;
}

// -------------------------------------------------------------------

// BaseBody

// optional bytes msg_data = 1;
inline bool BaseBody::has_msg_data() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BaseBody::set_has_msg_data() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BaseBody::clear_has_msg_data() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BaseBody::clear_msg_data() {
  if (msg_data_ != &::google::protobuf::internal::kEmptyString) {
    msg_data_->clear();
  }
  clear_has_msg_data();
}
inline const ::std::string& BaseBody::msg_data() const {
  return *msg_data_;
}
inline void BaseBody::set_msg_data(const ::std::string& value) {
  set_has_msg_data();
  if (msg_data_ == &::google::protobuf::internal::kEmptyString) {
    msg_data_ = new ::std::string;
  }
  msg_data_->assign(value);
}
inline void BaseBody::set_msg_data(const char* value) {
  set_has_msg_data();
  if (msg_data_ == &::google::protobuf::internal::kEmptyString) {
    msg_data_ = new ::std::string;
  }
  msg_data_->assign(value);
}
inline void BaseBody::set_msg_data(const void* value, size_t size) {
  set_has_msg_data();
  if (msg_data_ == &::google::protobuf::internal::kEmptyString) {
    msg_data_ = new ::std::string;
  }
  msg_data_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BaseBody::mutable_msg_data() {
  set_has_msg_data();
  if (msg_data_ == &::google::protobuf::internal::kEmptyString) {
    msg_data_ = new ::std::string;
  }
  return msg_data_;
}
inline ::std::string* BaseBody::release_msg_data() {
  clear_has_msg_data();
  if (msg_data_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = msg_data_;
    msg_data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// BaseMsg

// required .QuoteProto.BaseHead head = 1;
inline bool BaseMsg::has_head() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void BaseMsg::set_has_head() {
  _has_bits_[0] |= 0x00000001u;
}
inline void BaseMsg::clear_has_head() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void BaseMsg::clear_head() {
  if (head_ != NULL) head_->::QuoteProto::BaseHead::Clear();
  clear_has_head();
}
inline const ::QuoteProto::BaseHead& BaseMsg::head() const {
  return head_ != NULL ? *head_ : *default_instance_->head_;
}
inline ::QuoteProto::BaseHead* BaseMsg::mutable_head() {
  set_has_head();
  if (head_ == NULL) head_ = new ::QuoteProto::BaseHead;
  return head_;
}
inline ::QuoteProto::BaseHead* BaseMsg::release_head() {
  clear_has_head();
  ::QuoteProto::BaseHead* temp = head_;
  head_ = NULL;
  return temp;
}

// required .QuoteProto.BaseBody body = 2;
inline bool BaseMsg::has_body() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void BaseMsg::set_has_body() {
  _has_bits_[0] |= 0x00000002u;
}
inline void BaseMsg::clear_has_body() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void BaseMsg::clear_body() {
  if (body_ != NULL) body_->::QuoteProto::BaseBody::Clear();
  clear_has_body();
}
inline const ::QuoteProto::BaseBody& BaseMsg::body() const {
  return body_ != NULL ? *body_ : *default_instance_->body_;
}
inline ::QuoteProto::BaseBody* BaseMsg::mutable_body() {
  set_has_body();
  if (body_ == NULL) body_ = new ::QuoteProto::BaseBody;
  return body_;
}
inline ::QuoteProto::BaseBody* BaseMsg::release_body() {
  clear_has_body();
  ::QuoteProto::BaseBody* temp = body_;
  body_ = NULL;
  return temp;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< QuoteProto::EnumCmdType>() {
  return QuoteProto::EnumCmdType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_base_5fnews_2eproto__INCLUDED
