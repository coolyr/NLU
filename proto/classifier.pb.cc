// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "proto/classifier.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace dialog {
namespace qp {

namespace {

const ::google::protobuf::Descriptor* FeatureElement_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  FeatureElement_reflection_ = NULL;
const ::google::protobuf::Descriptor* Feature_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Feature_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_proto_2fclassifier_2eproto() {
  protobuf_AddDesc_proto_2fclassifier_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "proto/classifier.proto");
  GOOGLE_CHECK(file != NULL);
  FeatureElement_descriptor_ = file->message_type(0);
  static const int FeatureElement_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FeatureElement, key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FeatureElement, count_),
  };
  FeatureElement_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      FeatureElement_descriptor_,
      FeatureElement::default_instance_,
      FeatureElement_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FeatureElement, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FeatureElement, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(FeatureElement));
  Feature_descriptor_ = file->message_type(1);
  static const int Feature_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Feature, label_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Feature, elements_),
  };
  Feature_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Feature_descriptor_,
      Feature::default_instance_,
      Feature_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Feature, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Feature, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Feature));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_proto_2fclassifier_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    FeatureElement_descriptor_, &FeatureElement::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Feature_descriptor_, &Feature::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_proto_2fclassifier_2eproto() {
  delete FeatureElement::default_instance_;
  delete FeatureElement_reflection_;
  delete Feature::default_instance_;
  delete Feature_reflection_;
}

void protobuf_AddDesc_proto_2fclassifier_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026proto/classifier.proto\022\tdialog.qp\",\n\016F"
    "eatureElement\022\013\n\003key\030\001 \002(\t\022\r\n\005count\030\002 \002("
    "\r\"E\n\007Feature\022\r\n\005label\030\001 \002(\t\022+\n\010elements\030"
    "\002 \003(\0132\031.dialog.qp.FeatureElement", 152);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "proto/classifier.proto", &protobuf_RegisterTypes);
  FeatureElement::default_instance_ = new FeatureElement();
  Feature::default_instance_ = new Feature();
  FeatureElement::default_instance_->InitAsDefaultInstance();
  Feature::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_proto_2fclassifier_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_proto_2fclassifier_2eproto {
  StaticDescriptorInitializer_proto_2fclassifier_2eproto() {
    protobuf_AddDesc_proto_2fclassifier_2eproto();
  }
} static_descriptor_initializer_proto_2fclassifier_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int FeatureElement::kKeyFieldNumber;
const int FeatureElement::kCountFieldNumber;
#endif  // !_MSC_VER

FeatureElement::FeatureElement()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void FeatureElement::InitAsDefaultInstance() {
}

FeatureElement::FeatureElement(const FeatureElement& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void FeatureElement::SharedCtor() {
  _cached_size_ = 0;
  key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  count_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FeatureElement::~FeatureElement() {
  SharedDtor();
}

void FeatureElement::SharedDtor() {
  if (key_ != &::google::protobuf::internal::kEmptyString) {
    delete key_;
  }
  if (this != default_instance_) {
  }
}

void FeatureElement::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FeatureElement::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FeatureElement_descriptor_;
}

const FeatureElement& FeatureElement::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_proto_2fclassifier_2eproto();  return *default_instance_;
}

FeatureElement* FeatureElement::default_instance_ = NULL;

FeatureElement* FeatureElement::New() const {
  return new FeatureElement;
}

void FeatureElement::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_key()) {
      if (key_ != &::google::protobuf::internal::kEmptyString) {
        key_->clear();
      }
    }
    count_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool FeatureElement::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string key = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->key().data(), this->key().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_count;
        break;
      }
      
      // required uint32 count = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_count:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &count_)));
          set_has_count();
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

void FeatureElement::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string key = 1;
  if (has_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->key().data(), this->key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->key(), output);
  }
  
  // required uint32 count = 2;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->count(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* FeatureElement::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string key = 1;
  if (has_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->key().data(), this->key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->key(), target);
  }
  
  // required uint32 count = 2;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->count(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int FeatureElement::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string key = 1;
    if (has_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->key());
    }
    
    // required uint32 count = 2;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->count());
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

void FeatureElement::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const FeatureElement* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const FeatureElement*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void FeatureElement::MergeFrom(const FeatureElement& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_key()) {
      set_key(from.key());
    }
    if (from.has_count()) {
      set_count(from.count());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void FeatureElement::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FeatureElement::CopyFrom(const FeatureElement& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FeatureElement::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void FeatureElement::Swap(FeatureElement* other) {
  if (other != this) {
    std::swap(key_, other->key_);
    std::swap(count_, other->count_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata FeatureElement::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = FeatureElement_descriptor_;
  metadata.reflection = FeatureElement_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Feature::kLabelFieldNumber;
const int Feature::kElementsFieldNumber;
#endif  // !_MSC_VER

Feature::Feature()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Feature::InitAsDefaultInstance() {
}

Feature::Feature(const Feature& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Feature::SharedCtor() {
  _cached_size_ = 0;
  label_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Feature::~Feature() {
  SharedDtor();
}

void Feature::SharedDtor() {
  if (label_ != &::google::protobuf::internal::kEmptyString) {
    delete label_;
  }
  if (this != default_instance_) {
  }
}

void Feature::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Feature::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Feature_descriptor_;
}

const Feature& Feature::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_proto_2fclassifier_2eproto();  return *default_instance_;
}

Feature* Feature::default_instance_ = NULL;

Feature* Feature::New() const {
  return new Feature;
}

void Feature::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_label()) {
      if (label_ != &::google::protobuf::internal::kEmptyString) {
        label_->clear();
      }
    }
  }
  elements_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Feature::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string label = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_label()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->label().data(), this->label().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_elements;
        break;
      }
      
      // repeated .dialog.qp.FeatureElement elements = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_elements:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_elements()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_elements;
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

void Feature::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string label = 1;
  if (has_label()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->label().data(), this->label().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->label(), output);
  }
  
  // repeated .dialog.qp.FeatureElement elements = 2;
  for (int i = 0; i < this->elements_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->elements(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Feature::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string label = 1;
  if (has_label()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->label().data(), this->label().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->label(), target);
  }
  
  // repeated .dialog.qp.FeatureElement elements = 2;
  for (int i = 0; i < this->elements_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->elements(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Feature::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string label = 1;
    if (has_label()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->label());
    }
    
  }
  // repeated .dialog.qp.FeatureElement elements = 2;
  total_size += 1 * this->elements_size();
  for (int i = 0; i < this->elements_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->elements(i));
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

void Feature::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Feature* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Feature*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Feature::MergeFrom(const Feature& from) {
  GOOGLE_CHECK_NE(&from, this);
  elements_.MergeFrom(from.elements_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_label()) {
      set_label(from.label());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Feature::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Feature::CopyFrom(const Feature& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Feature::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  for (int i = 0; i < elements_size(); i++) {
    if (!this->elements(i).IsInitialized()) return false;
  }
  return true;
}

void Feature::Swap(Feature* other) {
  if (other != this) {
    std::swap(label_, other->label_);
    elements_.Swap(&other->elements_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Feature::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Feature_descriptor_;
  metadata.reflection = Feature_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace qp
}  // namespace dialog

// @@protoc_insertion_point(global_scope)
