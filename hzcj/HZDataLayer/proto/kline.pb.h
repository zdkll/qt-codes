// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: kline.proto

#ifndef PROTOBUF_kline_2eproto__INCLUDED
#define PROTOBUF_kline_2eproto__INCLUDED

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
#include "public_message.pb.h"
// @@protoc_insertion_point(includes)

namespace QuoteProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_kline_2eproto();
void protobuf_AssignDesc_kline_2eproto();
void protobuf_ShutdownFile_kline_2eproto();

class GetKLineRequest;
class GetKLineResponse;
class GetTimeKLineRequest;
class GetTimeKLineResponse;

// ===================================================================

class GetKLineRequest : public ::google::protobuf::Message {
 public:
  GetKLineRequest();
  virtual ~GetKLineRequest();
  
  GetKLineRequest(const GetKLineRequest& from);
  
  inline GetKLineRequest& operator=(const GetKLineRequest& from) {
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
  static const GetKLineRequest& default_instance();
  
  void Swap(GetKLineRequest* other);
  
  // implements Message ----------------------------------------------
  
  GetKLineRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetKLineRequest& from);
  void MergeFrom(const GetKLineRequest& from);
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
  
  // required string stock_name = 1;
  inline bool has_stock_name() const;
  inline void clear_stock_name();
  static const int kStockNameFieldNumber = 1;
  inline const ::std::string& stock_name() const;
  inline void set_stock_name(const ::std::string& value);
  inline void set_stock_name(const char* value);
  inline void set_stock_name(const char* value, size_t size);
  inline ::std::string* mutable_stock_name();
  inline ::std::string* release_stock_name();
  
  // required int32 kline_type = 2;
  inline bool has_kline_type() const;
  inline void clear_kline_type();
  static const int kKlineTypeFieldNumber = 2;
  inline ::google::protobuf::int32 kline_type() const;
  inline void set_kline_type(::google::protobuf::int32 value);
  
  // required int32 start_time = 3;
  inline bool has_start_time() const;
  inline void clear_start_time();
  static const int kStartTimeFieldNumber = 3;
  inline ::google::protobuf::int32 start_time() const;
  inline void set_start_time(::google::protobuf::int32 value);
  
  // required int32 count = 4;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 4;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);
  
  // required bool is_desc = 5;
  inline bool has_is_desc() const;
  inline void clear_is_desc();
  static const int kIsDescFieldNumber = 5;
  inline bool is_desc() const;
  inline void set_is_desc(bool value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetKLineRequest)
 private:
  inline void set_has_stock_name();
  inline void clear_has_stock_name();
  inline void set_has_kline_type();
  inline void clear_has_kline_type();
  inline void set_has_start_time();
  inline void clear_has_start_time();
  inline void set_has_count();
  inline void clear_has_count();
  inline void set_has_is_desc();
  inline void clear_has_is_desc();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* stock_name_;
  ::google::protobuf::int32 kline_type_;
  ::google::protobuf::int32 start_time_;
  ::google::protobuf::int32 count_;
  bool is_desc_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void  protobuf_AddDesc_kline_2eproto();
  friend void protobuf_AssignDesc_kline_2eproto();
  friend void protobuf_ShutdownFile_kline_2eproto();
  
  void InitAsDefaultInstance();
  static GetKLineRequest* default_instance_;
};
// -------------------------------------------------------------------

class GetKLineResponse : public ::google::protobuf::Message {
 public:
  GetKLineResponse();
  virtual ~GetKLineResponse();
  
  GetKLineResponse(const GetKLineResponse& from);
  
  inline GetKLineResponse& operator=(const GetKLineResponse& from) {
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
  static const GetKLineResponse& default_instance();
  
  void Swap(GetKLineResponse* other);
  
  // implements Message ----------------------------------------------
  
  GetKLineResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetKLineResponse& from);
  void MergeFrom(const GetKLineResponse& from);
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
  
  // repeated .QuoteProto.KLine kline_info = 2;
  inline int kline_info_size() const;
  inline void clear_kline_info();
  static const int kKlineInfoFieldNumber = 2;
  inline const ::QuoteProto::KLine& kline_info(int index) const;
  inline ::QuoteProto::KLine* mutable_kline_info(int index);
  inline ::QuoteProto::KLine* add_kline_info();
  inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >&
      kline_info() const;
  inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >*
      mutable_kline_info();
  
  // optional string stock_code = 3;
  inline bool has_stock_code() const;
  inline void clear_stock_code();
  static const int kStockCodeFieldNumber = 3;
  inline const ::std::string& stock_code() const;
  inline void set_stock_code(const ::std::string& value);
  inline void set_stock_code(const char* value);
  inline void set_stock_code(const char* value, size_t size);
  inline ::std::string* mutable_stock_code();
  inline ::std::string* release_stock_code();
  
  // optional int32 kline_type = 4;
  inline bool has_kline_type() const;
  inline void clear_kline_type();
  static const int kKlineTypeFieldNumber = 4;
  inline ::google::protobuf::int32 kline_type() const;
  inline void set_kline_type(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetKLineResponse)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_stock_code();
  inline void clear_has_stock_code();
  inline void set_has_kline_type();
  inline void clear_has_kline_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::QuoteProto::Ret* ret_;
  ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine > kline_info_;
  ::std::string* stock_code_;
  ::google::protobuf::int32 kline_type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_kline_2eproto();
  friend void protobuf_AssignDesc_kline_2eproto();
  friend void protobuf_ShutdownFile_kline_2eproto();
  
  void InitAsDefaultInstance();
  static GetKLineResponse* default_instance_;
};
// -------------------------------------------------------------------

class GetTimeKLineRequest : public ::google::protobuf::Message {
 public:
  GetTimeKLineRequest();
  virtual ~GetTimeKLineRequest();
  
  GetTimeKLineRequest(const GetTimeKLineRequest& from);
  
  inline GetTimeKLineRequest& operator=(const GetTimeKLineRequest& from) {
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
  static const GetTimeKLineRequest& default_instance();
  
  void Swap(GetTimeKLineRequest* other);
  
  // implements Message ----------------------------------------------
  
  GetTimeKLineRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetTimeKLineRequest& from);
  void MergeFrom(const GetTimeKLineRequest& from);
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
  
  // required string stock_name = 1;
  inline bool has_stock_name() const;
  inline void clear_stock_name();
  static const int kStockNameFieldNumber = 1;
  inline const ::std::string& stock_name() const;
  inline void set_stock_name(const ::std::string& value);
  inline void set_stock_name(const char* value);
  inline void set_stock_name(const char* value, size_t size);
  inline ::std::string* mutable_stock_name();
  inline ::std::string* release_stock_name();
  
  // required int32 start_time = 2;
  inline bool has_start_time() const;
  inline void clear_start_time();
  static const int kStartTimeFieldNumber = 2;
  inline ::google::protobuf::int32 start_time() const;
  inline void set_start_time(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetTimeKLineRequest)
 private:
  inline void set_has_stock_name();
  inline void clear_has_stock_name();
  inline void set_has_start_time();
  inline void clear_has_start_time();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* stock_name_;
  ::google::protobuf::int32 start_time_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_kline_2eproto();
  friend void protobuf_AssignDesc_kline_2eproto();
  friend void protobuf_ShutdownFile_kline_2eproto();
  
  void InitAsDefaultInstance();
  static GetTimeKLineRequest* default_instance_;
};
// -------------------------------------------------------------------

class GetTimeKLineResponse : public ::google::protobuf::Message {
 public:
  GetTimeKLineResponse();
  virtual ~GetTimeKLineResponse();
  
  GetTimeKLineResponse(const GetTimeKLineResponse& from);
  
  inline GetTimeKLineResponse& operator=(const GetTimeKLineResponse& from) {
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
  static const GetTimeKLineResponse& default_instance();
  
  void Swap(GetTimeKLineResponse* other);
  
  // implements Message ----------------------------------------------
  
  GetTimeKLineResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetTimeKLineResponse& from);
  void MergeFrom(const GetTimeKLineResponse& from);
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
  
  // repeated .QuoteProto.KLine kline_info = 2;
  inline int kline_info_size() const;
  inline void clear_kline_info();
  static const int kKlineInfoFieldNumber = 2;
  inline const ::QuoteProto::KLine& kline_info(int index) const;
  inline ::QuoteProto::KLine* mutable_kline_info(int index);
  inline ::QuoteProto::KLine* add_kline_info();
  inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >&
      kline_info() const;
  inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >*
      mutable_kline_info();
  
  // optional string stock_code = 3;
  inline bool has_stock_code() const;
  inline void clear_stock_code();
  static const int kStockCodeFieldNumber = 3;
  inline const ::std::string& stock_code() const;
  inline void set_stock_code(const ::std::string& value);
  inline void set_stock_code(const char* value);
  inline void set_stock_code(const char* value, size_t size);
  inline ::std::string* mutable_stock_code();
  inline ::std::string* release_stock_code();
  
  // optional int32 kline_type = 4;
  inline bool has_kline_type() const;
  inline void clear_kline_type();
  static const int kKlineTypeFieldNumber = 4;
  inline ::google::protobuf::int32 kline_type() const;
  inline void set_kline_type(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetTimeKLineResponse)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_stock_code();
  inline void clear_has_stock_code();
  inline void set_has_kline_type();
  inline void clear_has_kline_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::QuoteProto::Ret* ret_;
  ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine > kline_info_;
  ::std::string* stock_code_;
  ::google::protobuf::int32 kline_type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_kline_2eproto();
  friend void protobuf_AssignDesc_kline_2eproto();
  friend void protobuf_ShutdownFile_kline_2eproto();
  
  void InitAsDefaultInstance();
  static GetTimeKLineResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// GetKLineRequest

// required string stock_name = 1;
inline bool GetKLineRequest::has_stock_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetKLineRequest::set_has_stock_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetKLineRequest::clear_has_stock_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetKLineRequest::clear_stock_name() {
  if (stock_name_ != &::google::protobuf::internal::kEmptyString) {
    stock_name_->clear();
  }
  clear_has_stock_name();
}
inline const ::std::string& GetKLineRequest::stock_name() const {
  return *stock_name_;
}
inline void GetKLineRequest::set_stock_name(const ::std::string& value) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(value);
}
inline void GetKLineRequest::set_stock_name(const char* value) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(value);
}
inline void GetKLineRequest::set_stock_name(const char* value, size_t size) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GetKLineRequest::mutable_stock_name() {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  return stock_name_;
}
inline ::std::string* GetKLineRequest::release_stock_name() {
  clear_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = stock_name_;
    stock_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required int32 kline_type = 2;
inline bool GetKLineRequest::has_kline_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GetKLineRequest::set_has_kline_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GetKLineRequest::clear_has_kline_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GetKLineRequest::clear_kline_type() {
  kline_type_ = 0;
  clear_has_kline_type();
}
inline ::google::protobuf::int32 GetKLineRequest::kline_type() const {
  return kline_type_;
}
inline void GetKLineRequest::set_kline_type(::google::protobuf::int32 value) {
  set_has_kline_type();
  kline_type_ = value;
}

// required int32 start_time = 3;
inline bool GetKLineRequest::has_start_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GetKLineRequest::set_has_start_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GetKLineRequest::clear_has_start_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GetKLineRequest::clear_start_time() {
  start_time_ = 0;
  clear_has_start_time();
}
inline ::google::protobuf::int32 GetKLineRequest::start_time() const {
  return start_time_;
}
inline void GetKLineRequest::set_start_time(::google::protobuf::int32 value) {
  set_has_start_time();
  start_time_ = value;
}

// required int32 count = 4;
inline bool GetKLineRequest::has_count() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GetKLineRequest::set_has_count() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GetKLineRequest::clear_has_count() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GetKLineRequest::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 GetKLineRequest::count() const {
  return count_;
}
inline void GetKLineRequest::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// required bool is_desc = 5;
inline bool GetKLineRequest::has_is_desc() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GetKLineRequest::set_has_is_desc() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GetKLineRequest::clear_has_is_desc() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GetKLineRequest::clear_is_desc() {
  is_desc_ = false;
  clear_has_is_desc();
}
inline bool GetKLineRequest::is_desc() const {
  return is_desc_;
}
inline void GetKLineRequest::set_is_desc(bool value) {
  set_has_is_desc();
  is_desc_ = value;
}

// -------------------------------------------------------------------

// GetKLineResponse

// required .QuoteProto.Ret ret = 1;
inline bool GetKLineResponse::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetKLineResponse::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetKLineResponse::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetKLineResponse::clear_ret() {
  if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
  clear_has_ret();
}
inline const ::QuoteProto::Ret& GetKLineResponse::ret() const {
  return ret_ != NULL ? *ret_ : *default_instance_->ret_;
}
inline ::QuoteProto::Ret* GetKLineResponse::mutable_ret() {
  set_has_ret();
  if (ret_ == NULL) ret_ = new ::QuoteProto::Ret;
  return ret_;
}
inline ::QuoteProto::Ret* GetKLineResponse::release_ret() {
  clear_has_ret();
  ::QuoteProto::Ret* temp = ret_;
  ret_ = NULL;
  return temp;
}

// repeated .QuoteProto.KLine kline_info = 2;
inline int GetKLineResponse::kline_info_size() const {
  return kline_info_.size();
}
inline void GetKLineResponse::clear_kline_info() {
  kline_info_.Clear();
}
inline const ::QuoteProto::KLine& GetKLineResponse::kline_info(int index) const {
  return kline_info_.Get(index);
}
inline ::QuoteProto::KLine* GetKLineResponse::mutable_kline_info(int index) {
  return kline_info_.Mutable(index);
}
inline ::QuoteProto::KLine* GetKLineResponse::add_kline_info() {
  return kline_info_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >&
GetKLineResponse::kline_info() const {
  return kline_info_;
}
inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >*
GetKLineResponse::mutable_kline_info() {
  return &kline_info_;
}

// optional string stock_code = 3;
inline bool GetKLineResponse::has_stock_code() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GetKLineResponse::set_has_stock_code() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GetKLineResponse::clear_has_stock_code() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GetKLineResponse::clear_stock_code() {
  if (stock_code_ != &::google::protobuf::internal::kEmptyString) {
    stock_code_->clear();
  }
  clear_has_stock_code();
}
inline const ::std::string& GetKLineResponse::stock_code() const {
  return *stock_code_;
}
inline void GetKLineResponse::set_stock_code(const ::std::string& value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetKLineResponse::set_stock_code(const char* value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetKLineResponse::set_stock_code(const char* value, size_t size) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GetKLineResponse::mutable_stock_code() {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  return stock_code_;
}
inline ::std::string* GetKLineResponse::release_stock_code() {
  clear_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = stock_code_;
    stock_code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 kline_type = 4;
inline bool GetKLineResponse::has_kline_type() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GetKLineResponse::set_has_kline_type() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GetKLineResponse::clear_has_kline_type() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GetKLineResponse::clear_kline_type() {
  kline_type_ = 0;
  clear_has_kline_type();
}
inline ::google::protobuf::int32 GetKLineResponse::kline_type() const {
  return kline_type_;
}
inline void GetKLineResponse::set_kline_type(::google::protobuf::int32 value) {
  set_has_kline_type();
  kline_type_ = value;
}

// -------------------------------------------------------------------

// GetTimeKLineRequest

// required string stock_name = 1;
inline bool GetTimeKLineRequest::has_stock_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetTimeKLineRequest::set_has_stock_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetTimeKLineRequest::clear_has_stock_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetTimeKLineRequest::clear_stock_name() {
  if (stock_name_ != &::google::protobuf::internal::kEmptyString) {
    stock_name_->clear();
  }
  clear_has_stock_name();
}
inline const ::std::string& GetTimeKLineRequest::stock_name() const {
  return *stock_name_;
}
inline void GetTimeKLineRequest::set_stock_name(const ::std::string& value) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(value);
}
inline void GetTimeKLineRequest::set_stock_name(const char* value) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(value);
}
inline void GetTimeKLineRequest::set_stock_name(const char* value, size_t size) {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  stock_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GetTimeKLineRequest::mutable_stock_name() {
  set_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    stock_name_ = new ::std::string;
  }
  return stock_name_;
}
inline ::std::string* GetTimeKLineRequest::release_stock_name() {
  clear_has_stock_name();
  if (stock_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = stock_name_;
    stock_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required int32 start_time = 2;
inline bool GetTimeKLineRequest::has_start_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GetTimeKLineRequest::set_has_start_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GetTimeKLineRequest::clear_has_start_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GetTimeKLineRequest::clear_start_time() {
  start_time_ = 0;
  clear_has_start_time();
}
inline ::google::protobuf::int32 GetTimeKLineRequest::start_time() const {
  return start_time_;
}
inline void GetTimeKLineRequest::set_start_time(::google::protobuf::int32 value) {
  set_has_start_time();
  start_time_ = value;
}

// -------------------------------------------------------------------

// GetTimeKLineResponse

// required .QuoteProto.Ret ret = 1;
inline bool GetTimeKLineResponse::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetTimeKLineResponse::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetTimeKLineResponse::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetTimeKLineResponse::clear_ret() {
  if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
  clear_has_ret();
}
inline const ::QuoteProto::Ret& GetTimeKLineResponse::ret() const {
  return ret_ != NULL ? *ret_ : *default_instance_->ret_;
}
inline ::QuoteProto::Ret* GetTimeKLineResponse::mutable_ret() {
  set_has_ret();
  if (ret_ == NULL) ret_ = new ::QuoteProto::Ret;
  return ret_;
}
inline ::QuoteProto::Ret* GetTimeKLineResponse::release_ret() {
  clear_has_ret();
  ::QuoteProto::Ret* temp = ret_;
  ret_ = NULL;
  return temp;
}

// repeated .QuoteProto.KLine kline_info = 2;
inline int GetTimeKLineResponse::kline_info_size() const {
  return kline_info_.size();
}
inline void GetTimeKLineResponse::clear_kline_info() {
  kline_info_.Clear();
}
inline const ::QuoteProto::KLine& GetTimeKLineResponse::kline_info(int index) const {
  return kline_info_.Get(index);
}
inline ::QuoteProto::KLine* GetTimeKLineResponse::mutable_kline_info(int index) {
  return kline_info_.Mutable(index);
}
inline ::QuoteProto::KLine* GetTimeKLineResponse::add_kline_info() {
  return kline_info_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >&
GetTimeKLineResponse::kline_info() const {
  return kline_info_;
}
inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::KLine >*
GetTimeKLineResponse::mutable_kline_info() {
  return &kline_info_;
}

// optional string stock_code = 3;
inline bool GetTimeKLineResponse::has_stock_code() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GetTimeKLineResponse::set_has_stock_code() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GetTimeKLineResponse::clear_has_stock_code() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GetTimeKLineResponse::clear_stock_code() {
  if (stock_code_ != &::google::protobuf::internal::kEmptyString) {
    stock_code_->clear();
  }
  clear_has_stock_code();
}
inline const ::std::string& GetTimeKLineResponse::stock_code() const {
  return *stock_code_;
}
inline void GetTimeKLineResponse::set_stock_code(const ::std::string& value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetTimeKLineResponse::set_stock_code(const char* value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetTimeKLineResponse::set_stock_code(const char* value, size_t size) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GetTimeKLineResponse::mutable_stock_code() {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  return stock_code_;
}
inline ::std::string* GetTimeKLineResponse::release_stock_code() {
  clear_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = stock_code_;
    stock_code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 kline_type = 4;
inline bool GetTimeKLineResponse::has_kline_type() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GetTimeKLineResponse::set_has_kline_type() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GetTimeKLineResponse::clear_has_kline_type() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GetTimeKLineResponse::clear_kline_type() {
  kline_type_ = 0;
  clear_has_kline_type();
}
inline ::google::protobuf::int32 GetTimeKLineResponse::kline_type() const {
  return kline_type_;
}
inline void GetTimeKLineResponse::set_kline_type(::google::protobuf::int32 value) {
  set_has_kline_type();
  kline_type_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_kline_2eproto__INCLUDED