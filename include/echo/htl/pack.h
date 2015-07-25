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

  Value& detail_pack_value() & { return reinterpret_cast<Value&>(*this); }
  const Value& detail_pack_value() const & {
    return reinterpret_cast<const Value&>(*this);
  }
  Value&& detail_pack_value() && { return reinterpret_cast<Value&&>(*this); }
};

template <class Tag, class Value>
struct PackBase<Tag, Value, false> {
  CONCEPT_MEMBER_REQUIRES(std::is_default_constructible<Value>::value)
  PackBase() {}

  template <class OtherValue,
            CONCEPT_REQUIRES(std::is_constructible<Value, OtherValue>::value)>
  PackBase(OtherValue&& other_value)
      : _detail_pack_value(std::forward<OtherValue>(other_value)) {}

  auto& detail_pack_value() & { return _detail_pack_value; }
  auto& detail_pack_value() const & { return _detail_pack_value; }
  auto&& detail_pack_value() && { return std::move(_detail_pack_value); }

 private:
  Value _detail_pack_value;
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
  return pack.detail_pack_value();
}

template <class Tag, class Value>
decltype(auto) unpack(const Pack<Tag, Value>& pack) {
  return pack.detail_pack_value();
}

template <class Tag, class Value>
decltype(auto) unpack(Pack<Tag, Value>&& pack) {
  return std::move(pack).detail_pack_value();
}
}

namespace htl {

using empty_base_class_namespace::Pack;
using empty_base_class_namespace::unpack;
}
}

#undef DETAIL_NS
