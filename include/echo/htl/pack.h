#pragma once

#define DETAIL_NS detail_pack

#include <type_traits>
#include <utility>
#include <echo/concept.h>

namespace echo {
// Pack is intended to be used with empty-base-class-optimization, so prevent
// unwanted ADL it uses a separate and minimal namespace
namespace empty_base_class_namespace {

//------------------------------------------------------------------------------
// PackBase
//------------------------------------------------------------------------------
namespace DETAIL_NS {

template <class Tag, class Value, bool IsValueEmpty>
struct PackBase {};

template <class Tag, class Value>
struct PackBase<Tag, Value, true> {
  CONCEPT_MEMBER_REQUIRES(std::is_default_constructible<Value>::value)
  PackBase() {}

  template <class OtherValue,
            CONCEPT_REQUIRES(std::is_constructible<Value, OtherValue>::value)>
  PackBase(OtherValue&& other_value) {}

  Value& value() & { return reinterpret_cast<Value&>(*this); }
  const Value& value() const & { return reinterpret_cast<const Value&>(*this); }
  Value&& value() && { return reinterpret_cast<Value&&>(*this); }
};

template <class Tag, class Value>
struct PackBase<Tag, Value, false> {
  CONCEPT_MEMBER_REQUIRES(std::is_default_constructible<Value>::value)
  PackBase() {}

  template <class OtherValue,
            CONCEPT_REQUIRES(std::is_constructible<Value, OtherValue>::value)>
  PackBase(OtherValue&& other_value)
      : _value(std::forward<OtherValue>(other_value)) {}

  auto& value() & { return _value; }
  auto& value() const & { return _value; }
  auto&& value() && { return std::move(_value); }

 private:
  Value _value;
};
}

//------------------------------------------------------------------------------
// Pack
//------------------------------------------------------------------------------
template <class Tag, class Value = Tag>
struct Pack : DETAIL_NS::PackBase<Tag, Value, std::is_empty<Value>::value> {
  using DETAIL_NS::PackBase<Tag, Value, std::is_empty<Value>::value>::PackBase;
};

//------------------------------------------------------------------------------
// unpack
//------------------------------------------------------------------------------
template <class Tag, class Value>
decltype(auto) unpack(Pack<Tag, Value>& pack) {
  return pack.value();
}

template <class Tag, class Value>
decltype(auto) unpack(const Pack<Tag, Value>& pack) {
  return pack.value();
}

template <class Tag, class Value>
decltype(auto) unpack(Pack<Tag, Value>&& pack) {
  return std::move(pack).value();
}
}

namespace htl {

using empty_base_class_namespace::Pack;
using empty_base_class_namespace::unpack;
}
}

#undef DETAIL_NS
