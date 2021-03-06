// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/classifier.proto

#ifndef PROTOBUF_proto_2fclassifier_2eproto__INCLUDED
#define PROTOBUF_proto_2fclassifier_2eproto__INCLUDED

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

namespace dialog {
namespace qp {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_proto_2fclassifier_2eproto();
void protobuf_AssignDesc_proto_2fclassifier_2eproto();
void protobuf_ShutdownFile_proto_2fclassifier_2eproto();

class FeatureElement;
class Feature;

// ===================================================================

class FeatureElement : public ::google::protobuf::Message {
 public:
  FeatureElement();
  virtual ~FeatureElement();
  
  FeatureElement(const FeatureElement& from);
  
  inline FeatureElement& operator=(const FeatureElement& from) {
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
  static const FeatureElement& default_instance();
  
  void Swap(FeatureElement* other);
  
  // implements Message ----------------------------------------------
  
  FeatureElement* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FeatureElement& from);
  void MergeFrom(const FeatureElement& from);
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
  
  // required string key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const char* value, size_t size);
  inline ::std::string* mutable_key();
  inline ::std::string* release_key();
  
  // required uint32 count = 2;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 2;
  inline ::google::protobuf::uint32 count() const;
  inline void set_count(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:dialog.qp.FeatureElement)
 private:
  inline void set_has_key();
  inline void clear_has_key();
  inline void set_has_count();
  inline void clear_has_count();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* key_;
  ::google::protobuf::uint32 count_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_proto_2fclassifier_2eproto();
  friend void protobuf_AssignDesc_proto_2fclassifier_2eproto();
  friend void protobuf_ShutdownFile_proto_2fclassifier_2eproto();
  
  void InitAsDefaultInstance();
  static FeatureElement* default_instance_;
};
// -------------------------------------------------------------------

class Feature : public ::google::protobuf::Message {
 public:
  Feature();
  virtual ~Feature();
  
  Feature(const Feature& from);
  
  inline Feature& operator=(const Feature& from) {
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
  static const Feature& default_instance();
  
  void Swap(Feature* other);
  
  // implements Message ----------------------------------------------
  
  Feature* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Feature& from);
  void MergeFrom(const Feature& from);
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
  
  // required string label = 1;
  inline bool has_label() const;
  inline void clear_label();
  static const int kLabelFieldNumber = 1;
  inline const ::std::string& label() const;
  inline void set_label(const ::std::string& value);
  inline void set_label(const char* value);
  inline void set_label(const char* value, size_t size);
  inline ::std::string* mutable_label();
  inline ::std::string* release_label();
  
  // repeated .dialog.qp.FeatureElement elements = 2;
  inline int elements_size() const;
  inline void clear_elements();
  static const int kElementsFieldNumber = 2;
  inline const ::dialog::qp::FeatureElement& elements(int index) const;
  inline ::dialog::qp::FeatureElement* mutable_elements(int index);
  inline ::dialog::qp::FeatureElement* add_elements();
  inline const ::google::protobuf::RepeatedPtrField< ::dialog::qp::FeatureElement >&
      elements() const;
  inline ::google::protobuf::RepeatedPtrField< ::dialog::qp::FeatureElement >*
      mutable_elements();
  
  // @@protoc_insertion_point(class_scope:dialog.qp.Feature)
 private:
  inline void set_has_label();
  inline void clear_has_label();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* label_;
  ::google::protobuf::RepeatedPtrField< ::dialog::qp::FeatureElement > elements_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_proto_2fclassifier_2eproto();
  friend void protobuf_AssignDesc_proto_2fclassifier_2eproto();
  friend void protobuf_ShutdownFile_proto_2fclassifier_2eproto();
  
  void InitAsDefaultInstance();
  static Feature* default_instance_;
};
// ===================================================================


// ===================================================================

// FeatureElement

// required string key = 1;
inline bool FeatureElement::has_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FeatureElement::set_has_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FeatureElement::clear_has_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FeatureElement::clear_key() {
  if (key_ != &::google::protobuf::internal::kEmptyString) {
    key_->clear();
  }
  clear_has_key();
}
inline const ::std::string& FeatureElement::key() const {
  return *key_;
}
inline void FeatureElement::set_key(const ::std::string& value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void FeatureElement::set_key(const char* value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void FeatureElement::set_key(const char* value, size_t size) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FeatureElement::mutable_key() {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  return key_;
}
inline ::std::string* FeatureElement::release_key() {
  clear_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = key_;
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required uint32 count = 2;
inline bool FeatureElement::has_count() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void FeatureElement::set_has_count() {
  _has_bits_[0] |= 0x00000002u;
}
inline void FeatureElement::clear_has_count() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void FeatureElement::clear_count() {
  count_ = 0u;
  clear_has_count();
}
inline ::google::protobuf::uint32 FeatureElement::count() const {
  return count_;
}
inline void FeatureElement::set_count(::google::protobuf::uint32 value) {
  set_has_count();
  count_ = value;
}

// -------------------------------------------------------------------

// Feature

// required string label = 1;
inline bool Feature::has_label() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Feature::set_has_label() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Feature::clear_has_label() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Feature::clear_label() {
  if (label_ != &::google::protobuf::internal::kEmptyString) {
    label_->clear();
  }
  clear_has_label();
}
inline const ::std::string& Feature::label() const {
  return *label_;
}
inline void Feature::set_label(const ::std::string& value) {
  set_has_label();
  if (label_ == &::google::protobuf::internal::kEmptyString) {
    label_ = new ::std::string;
  }
  label_->assign(value);
}
inline void Feature::set_label(const char* value) {
  set_has_label();
  if (label_ == &::google::protobuf::internal::kEmptyString) {
    label_ = new ::std::string;
  }
  label_->assign(value);
}
inline void Feature::set_label(const char* value, size_t size) {
  set_has_label();
  if (label_ == &::google::protobuf::internal::kEmptyString) {
    label_ = new ::std::string;
  }
  label_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Feature::mutable_label() {
  set_has_label();
  if (label_ == &::google::protobuf::internal::kEmptyString) {
    label_ = new ::std::string;
  }
  return label_;
}
inline ::std::string* Feature::release_label() {
  clear_has_label();
  if (label_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = label_;
    label_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .dialog.qp.FeatureElement elements = 2;
inline int Feature::elements_size() const {
  return elements_.size();
}
inline void Feature::clear_elements() {
  elements_.Clear();
}
inline const ::dialog::qp::FeatureElement& Feature::elements(int index) const {
  return elements_.Get(index);
}
inline ::dialog::qp::FeatureElement* Feature::mutable_elements(int index) {
  return elements_.Mutable(index);
}
inline ::dialog::qp::FeatureElement* Feature::add_elements() {
  return elements_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::dialog::qp::FeatureElement >&
Feature::elements() const {
  return elements_;
}
inline ::google::protobuf::RepeatedPtrField< ::dialog::qp::FeatureElement >*
Feature::mutable_elements() {
  return &elements_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace qp
}  // namespace dialog

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_proto_2fclassifier_2eproto__INCLUDED
