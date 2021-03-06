// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: zhibiao.proto

#ifndef PROTOBUF_zhibiao_2eproto__INCLUDED
#define PROTOBUF_zhibiao_2eproto__INCLUDED

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
void  protobuf_AddDesc_zhibiao_2eproto();
void protobuf_AssignDesc_zhibiao_2eproto();
void protobuf_ShutdownFile_zhibiao_2eproto();

class DotResult;
class ColumnResult;
class GetZhibiaoRequest;
class GetZhibiaoResponse;

enum ZhibiaoType {
  JDCJ = 1,
  CMFB = 2,
  QSGD = 3,
  HLQJ = 4,
  ZJKP = 5,
  CPTX = 6,
  HPM = 7,
  XQQX = 8,
  DQKJ = 9,
  DBGJ = 10,
  DBDJ = 11
};
bool ZhibiaoType_IsValid(int value);
const ZhibiaoType ZhibiaoType_MIN = JDCJ;
const ZhibiaoType ZhibiaoType_MAX = DBDJ;
const int ZhibiaoType_ARRAYSIZE = ZhibiaoType_MAX + 1;

const ::google::protobuf::EnumDescriptor* ZhibiaoType_descriptor();
inline const ::std::string& ZhibiaoType_Name(ZhibiaoType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ZhibiaoType_descriptor(), value);
}
inline bool ZhibiaoType_Parse(
    const ::std::string& name, ZhibiaoType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ZhibiaoType>(
    ZhibiaoType_descriptor(), name, value);
}
// ===================================================================

class DotResult : public ::google::protobuf::Message {
 public:
  DotResult();
  virtual ~DotResult();
  
  DotResult(const DotResult& from);
  
  inline DotResult& operator=(const DotResult& from) {
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
  static const DotResult& default_instance();
  
  void Swap(DotResult* other);
  
  // implements Message ----------------------------------------------
  
  DotResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DotResult& from);
  void MergeFrom(const DotResult& from);
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
  
  // required int32 upserttime_time = 1;
  inline bool has_upserttime_time() const;
  inline void clear_upserttime_time();
  static const int kUpserttimeTimeFieldNumber = 1;
  inline ::google::protobuf::int32 upserttime_time() const;
  inline void set_upserttime_time(::google::protobuf::int32 value);
  
  // required int32 stype = 2;
  inline bool has_stype() const;
  inline void clear_stype();
  static const int kStypeFieldNumber = 2;
  inline ::google::protobuf::int32 stype() const;
  inline void set_stype(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.DotResult)
 private:
  inline void set_has_upserttime_time();
  inline void clear_has_upserttime_time();
  inline void set_has_stype();
  inline void clear_has_stype();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 upserttime_time_;
  ::google::protobuf::int32 stype_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_zhibiao_2eproto();
  friend void protobuf_AssignDesc_zhibiao_2eproto();
  friend void protobuf_ShutdownFile_zhibiao_2eproto();
  
  void InitAsDefaultInstance();
  static DotResult* default_instance_;
};
// -------------------------------------------------------------------

class ColumnResult : public ::google::protobuf::Message {
 public:
  ColumnResult();
  virtual ~ColumnResult();
  
  ColumnResult(const ColumnResult& from);
  
  inline ColumnResult& operator=(const ColumnResult& from) {
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
  static const ColumnResult& default_instance();
  
  void Swap(ColumnResult* other);
  
  // implements Message ----------------------------------------------
  
  ColumnResult* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ColumnResult& from);
  void MergeFrom(const ColumnResult& from);
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
  
  // required int32 color_type = 1;
  inline bool has_color_type() const;
  inline void clear_color_type();
  static const int kColorTypeFieldNumber = 1;
  inline ::google::protobuf::int32 color_type() const;
  inline void set_color_type(::google::protobuf::int32 value);
  
  // required int32 upserttime_time = 2;
  inline bool has_upserttime_time() const;
  inline void clear_upserttime_time();
  static const int kUpserttimeTimeFieldNumber = 2;
  inline ::google::protobuf::int32 upserttime_time() const;
  inline void set_upserttime_time(::google::protobuf::int32 value);
  
  // required int32 high = 3;
  inline bool has_high() const;
  inline void clear_high();
  static const int kHighFieldNumber = 3;
  inline ::google::protobuf::int32 high() const;
  inline void set_high(::google::protobuf::int32 value);
  
  // optional int32 low = 4;
  inline bool has_low() const;
  inline void clear_low();
  static const int kLowFieldNumber = 4;
  inline ::google::protobuf::int32 low() const;
  inline void set_low(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.ColumnResult)
 private:
  inline void set_has_color_type();
  inline void clear_has_color_type();
  inline void set_has_upserttime_time();
  inline void clear_has_upserttime_time();
  inline void set_has_high();
  inline void clear_has_high();
  inline void set_has_low();
  inline void clear_has_low();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 color_type_;
  ::google::protobuf::int32 upserttime_time_;
  ::google::protobuf::int32 high_;
  ::google::protobuf::int32 low_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_zhibiao_2eproto();
  friend void protobuf_AssignDesc_zhibiao_2eproto();
  friend void protobuf_ShutdownFile_zhibiao_2eproto();
  
  void InitAsDefaultInstance();
  static ColumnResult* default_instance_;
};
// -------------------------------------------------------------------

class GetZhibiaoRequest : public ::google::protobuf::Message {
 public:
  GetZhibiaoRequest();
  virtual ~GetZhibiaoRequest();
  
  GetZhibiaoRequest(const GetZhibiaoRequest& from);
  
  inline GetZhibiaoRequest& operator=(const GetZhibiaoRequest& from) {
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
  static const GetZhibiaoRequest& default_instance();
  
  void Swap(GetZhibiaoRequest* other);
  
  // implements Message ----------------------------------------------
  
  GetZhibiaoRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetZhibiaoRequest& from);
  void MergeFrom(const GetZhibiaoRequest& from);
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
  
  // required string stock_code = 1;
  inline bool has_stock_code() const;
  inline void clear_stock_code();
  static const int kStockCodeFieldNumber = 1;
  inline const ::std::string& stock_code() const;
  inline void set_stock_code(const ::std::string& value);
  inline void set_stock_code(const char* value);
  inline void set_stock_code(const char* value, size_t size);
  inline ::std::string* mutable_stock_code();
  inline ::std::string* release_stock_code();
  
  // required .QuoteProto.ZhibiaoType ztype = 2;
  inline bool has_ztype() const;
  inline void clear_ztype();
  static const int kZtypeFieldNumber = 2;
  inline QuoteProto::ZhibiaoType ztype() const;
  inline void set_ztype(QuoteProto::ZhibiaoType value);
  
  // required int32 start_time = 3;
  inline bool has_start_time() const;
  inline void clear_start_time();
  static const int kStartTimeFieldNumber = 3;
  inline ::google::protobuf::int32 start_time() const;
  inline void set_start_time(::google::protobuf::int32 value);
  
  // required int32 end_time = 4;
  inline bool has_end_time() const;
  inline void clear_end_time();
  static const int kEndTimeFieldNumber = 4;
  inline ::google::protobuf::int32 end_time() const;
  inline void set_end_time(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetZhibiaoRequest)
 private:
  inline void set_has_stock_code();
  inline void clear_has_stock_code();
  inline void set_has_ztype();
  inline void clear_has_ztype();
  inline void set_has_start_time();
  inline void clear_has_start_time();
  inline void set_has_end_time();
  inline void clear_has_end_time();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* stock_code_;
  int ztype_;
  ::google::protobuf::int32 start_time_;
  ::google::protobuf::int32 end_time_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_zhibiao_2eproto();
  friend void protobuf_AssignDesc_zhibiao_2eproto();
  friend void protobuf_ShutdownFile_zhibiao_2eproto();
  
  void InitAsDefaultInstance();
  static GetZhibiaoRequest* default_instance_;
};
// -------------------------------------------------------------------

class GetZhibiaoResponse : public ::google::protobuf::Message {
 public:
  GetZhibiaoResponse();
  virtual ~GetZhibiaoResponse();
  
  GetZhibiaoResponse(const GetZhibiaoResponse& from);
  
  inline GetZhibiaoResponse& operator=(const GetZhibiaoResponse& from) {
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
  static const GetZhibiaoResponse& default_instance();
  
  void Swap(GetZhibiaoResponse* other);
  
  // implements Message ----------------------------------------------
  
  GetZhibiaoResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GetZhibiaoResponse& from);
  void MergeFrom(const GetZhibiaoResponse& from);
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
  
  // required .QuoteProto.ZhibiaoType ztype = 2;
  inline bool has_ztype() const;
  inline void clear_ztype();
  static const int kZtypeFieldNumber = 2;
  inline QuoteProto::ZhibiaoType ztype() const;
  inline void set_ztype(QuoteProto::ZhibiaoType value);
  
  // repeated .QuoteProto.DotResult dot_info = 3;
  inline int dot_info_size() const;
  inline void clear_dot_info();
  static const int kDotInfoFieldNumber = 3;
  inline const ::QuoteProto::DotResult& dot_info(int index) const;
  inline ::QuoteProto::DotResult* mutable_dot_info(int index);
  inline ::QuoteProto::DotResult* add_dot_info();
  inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::DotResult >&
      dot_info() const;
  inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::DotResult >*
      mutable_dot_info();
  
  // repeated .QuoteProto.ColumnResult col_info = 4;
  inline int col_info_size() const;
  inline void clear_col_info();
  static const int kColInfoFieldNumber = 4;
  inline const ::QuoteProto::ColumnResult& col_info(int index) const;
  inline ::QuoteProto::ColumnResult* mutable_col_info(int index);
  inline ::QuoteProto::ColumnResult* add_col_info();
  inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::ColumnResult >&
      col_info() const;
  inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::ColumnResult >*
      mutable_col_info();
  
  // @@protoc_insertion_point(class_scope:QuoteProto.GetZhibiaoResponse)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_ztype();
  inline void clear_has_ztype();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::QuoteProto::Ret* ret_;
  ::google::protobuf::RepeatedPtrField< ::QuoteProto::DotResult > dot_info_;
  ::google::protobuf::RepeatedPtrField< ::QuoteProto::ColumnResult > col_info_;
  int ztype_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_zhibiao_2eproto();
  friend void protobuf_AssignDesc_zhibiao_2eproto();
  friend void protobuf_ShutdownFile_zhibiao_2eproto();
  
  void InitAsDefaultInstance();
  static GetZhibiaoResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// DotResult

// required int32 upserttime_time = 1;
inline bool DotResult::has_upserttime_time() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DotResult::set_has_upserttime_time() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DotResult::clear_has_upserttime_time() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DotResult::clear_upserttime_time() {
  upserttime_time_ = 0;
  clear_has_upserttime_time();
}
inline ::google::protobuf::int32 DotResult::upserttime_time() const {
  return upserttime_time_;
}
inline void DotResult::set_upserttime_time(::google::protobuf::int32 value) {
  set_has_upserttime_time();
  upserttime_time_ = value;
}

// required int32 stype = 2;
inline bool DotResult::has_stype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DotResult::set_has_stype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DotResult::clear_has_stype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DotResult::clear_stype() {
  stype_ = 0;
  clear_has_stype();
}
inline ::google::protobuf::int32 DotResult::stype() const {
  return stype_;
}
inline void DotResult::set_stype(::google::protobuf::int32 value) {
  set_has_stype();
  stype_ = value;
}

// -------------------------------------------------------------------

// ColumnResult

// required int32 color_type = 1;
inline bool ColumnResult::has_color_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ColumnResult::set_has_color_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ColumnResult::clear_has_color_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ColumnResult::clear_color_type() {
  color_type_ = 0;
  clear_has_color_type();
}
inline ::google::protobuf::int32 ColumnResult::color_type() const {
  return color_type_;
}
inline void ColumnResult::set_color_type(::google::protobuf::int32 value) {
  set_has_color_type();
  color_type_ = value;
}

// required int32 upserttime_time = 2;
inline bool ColumnResult::has_upserttime_time() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ColumnResult::set_has_upserttime_time() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ColumnResult::clear_has_upserttime_time() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ColumnResult::clear_upserttime_time() {
  upserttime_time_ = 0;
  clear_has_upserttime_time();
}
inline ::google::protobuf::int32 ColumnResult::upserttime_time() const {
  return upserttime_time_;
}
inline void ColumnResult::set_upserttime_time(::google::protobuf::int32 value) {
  set_has_upserttime_time();
  upserttime_time_ = value;
}

// required int32 high = 3;
inline bool ColumnResult::has_high() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ColumnResult::set_has_high() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ColumnResult::clear_has_high() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ColumnResult::clear_high() {
  high_ = 0;
  clear_has_high();
}
inline ::google::protobuf::int32 ColumnResult::high() const {
  return high_;
}
inline void ColumnResult::set_high(::google::protobuf::int32 value) {
  set_has_high();
  high_ = value;
}

// optional int32 low = 4;
inline bool ColumnResult::has_low() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ColumnResult::set_has_low() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ColumnResult::clear_has_low() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ColumnResult::clear_low() {
  low_ = 0;
  clear_has_low();
}
inline ::google::protobuf::int32 ColumnResult::low() const {
  return low_;
}
inline void ColumnResult::set_low(::google::protobuf::int32 value) {
  set_has_low();
  low_ = value;
}

// -------------------------------------------------------------------

// GetZhibiaoRequest

// required string stock_code = 1;
inline bool GetZhibiaoRequest::has_stock_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetZhibiaoRequest::set_has_stock_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetZhibiaoRequest::clear_has_stock_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetZhibiaoRequest::clear_stock_code() {
  if (stock_code_ != &::google::protobuf::internal::kEmptyString) {
    stock_code_->clear();
  }
  clear_has_stock_code();
}
inline const ::std::string& GetZhibiaoRequest::stock_code() const {
  return *stock_code_;
}
inline void GetZhibiaoRequest::set_stock_code(const ::std::string& value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetZhibiaoRequest::set_stock_code(const char* value) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(value);
}
inline void GetZhibiaoRequest::set_stock_code(const char* value, size_t size) {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  stock_code_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GetZhibiaoRequest::mutable_stock_code() {
  set_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    stock_code_ = new ::std::string;
  }
  return stock_code_;
}
inline ::std::string* GetZhibiaoRequest::release_stock_code() {
  clear_has_stock_code();
  if (stock_code_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = stock_code_;
    stock_code_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required .QuoteProto.ZhibiaoType ztype = 2;
inline bool GetZhibiaoRequest::has_ztype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GetZhibiaoRequest::set_has_ztype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GetZhibiaoRequest::clear_has_ztype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GetZhibiaoRequest::clear_ztype() {
  ztype_ = 1;
  clear_has_ztype();
}
inline QuoteProto::ZhibiaoType GetZhibiaoRequest::ztype() const {
  return static_cast< QuoteProto::ZhibiaoType >(ztype_);
}
inline void GetZhibiaoRequest::set_ztype(QuoteProto::ZhibiaoType value) {
  GOOGLE_DCHECK(QuoteProto::ZhibiaoType_IsValid(value));
  set_has_ztype();
  ztype_ = value;
}

// required int32 start_time = 3;
inline bool GetZhibiaoRequest::has_start_time() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GetZhibiaoRequest::set_has_start_time() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GetZhibiaoRequest::clear_has_start_time() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GetZhibiaoRequest::clear_start_time() {
  start_time_ = 0;
  clear_has_start_time();
}
inline ::google::protobuf::int32 GetZhibiaoRequest::start_time() const {
  return start_time_;
}
inline void GetZhibiaoRequest::set_start_time(::google::protobuf::int32 value) {
  set_has_start_time();
  start_time_ = value;
}

// required int32 end_time = 4;
inline bool GetZhibiaoRequest::has_end_time() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GetZhibiaoRequest::set_has_end_time() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GetZhibiaoRequest::clear_has_end_time() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GetZhibiaoRequest::clear_end_time() {
  end_time_ = 0;
  clear_has_end_time();
}
inline ::google::protobuf::int32 GetZhibiaoRequest::end_time() const {
  return end_time_;
}
inline void GetZhibiaoRequest::set_end_time(::google::protobuf::int32 value) {
  set_has_end_time();
  end_time_ = value;
}

// -------------------------------------------------------------------

// GetZhibiaoResponse

// required .QuoteProto.Ret ret = 1;
inline bool GetZhibiaoResponse::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GetZhibiaoResponse::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GetZhibiaoResponse::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GetZhibiaoResponse::clear_ret() {
  if (ret_ != NULL) ret_->::QuoteProto::Ret::Clear();
  clear_has_ret();
}
inline const ::QuoteProto::Ret& GetZhibiaoResponse::ret() const {
  return ret_ != NULL ? *ret_ : *default_instance_->ret_;
}
inline ::QuoteProto::Ret* GetZhibiaoResponse::mutable_ret() {
  set_has_ret();
  if (ret_ == NULL) ret_ = new ::QuoteProto::Ret;
  return ret_;
}
inline ::QuoteProto::Ret* GetZhibiaoResponse::release_ret() {
  clear_has_ret();
  ::QuoteProto::Ret* temp = ret_;
  ret_ = NULL;
  return temp;
}

// required .QuoteProto.ZhibiaoType ztype = 2;
inline bool GetZhibiaoResponse::has_ztype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GetZhibiaoResponse::set_has_ztype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GetZhibiaoResponse::clear_has_ztype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GetZhibiaoResponse::clear_ztype() {
  ztype_ = 1;
  clear_has_ztype();
}
inline QuoteProto::ZhibiaoType GetZhibiaoResponse::ztype() const {
  return static_cast< QuoteProto::ZhibiaoType >(ztype_);
}
inline void GetZhibiaoResponse::set_ztype(QuoteProto::ZhibiaoType value) {
  GOOGLE_DCHECK(QuoteProto::ZhibiaoType_IsValid(value));
  set_has_ztype();
  ztype_ = value;
}

// repeated .QuoteProto.DotResult dot_info = 3;
inline int GetZhibiaoResponse::dot_info_size() const {
  return dot_info_.size();
}
inline void GetZhibiaoResponse::clear_dot_info() {
  dot_info_.Clear();
}
inline const ::QuoteProto::DotResult& GetZhibiaoResponse::dot_info(int index) const {
  return dot_info_.Get(index);
}
inline ::QuoteProto::DotResult* GetZhibiaoResponse::mutable_dot_info(int index) {
  return dot_info_.Mutable(index);
}
inline ::QuoteProto::DotResult* GetZhibiaoResponse::add_dot_info() {
  return dot_info_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::DotResult >&
GetZhibiaoResponse::dot_info() const {
  return dot_info_;
}
inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::DotResult >*
GetZhibiaoResponse::mutable_dot_info() {
  return &dot_info_;
}

// repeated .QuoteProto.ColumnResult col_info = 4;
inline int GetZhibiaoResponse::col_info_size() const {
  return col_info_.size();
}
inline void GetZhibiaoResponse::clear_col_info() {
  col_info_.Clear();
}
inline const ::QuoteProto::ColumnResult& GetZhibiaoResponse::col_info(int index) const {
  return col_info_.Get(index);
}
inline ::QuoteProto::ColumnResult* GetZhibiaoResponse::mutable_col_info(int index) {
  return col_info_.Mutable(index);
}
inline ::QuoteProto::ColumnResult* GetZhibiaoResponse::add_col_info() {
  return col_info_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::QuoteProto::ColumnResult >&
GetZhibiaoResponse::col_info() const {
  return col_info_;
}
inline ::google::protobuf::RepeatedPtrField< ::QuoteProto::ColumnResult >*
GetZhibiaoResponse::mutable_col_info() {
  return &col_info_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace QuoteProto

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< QuoteProto::ZhibiaoType>() {
  return QuoteProto::ZhibiaoType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_zhibiao_2eproto__INCLUDED
