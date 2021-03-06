#pragma once

#include <echo/concept.h>
#include <type_traits>
#include <algorithm>

namespace echo {
namespace htl {
//------------------------------------------------------------------------------
// integral_constant
//------------------------------------------------------------------------------
template <class T, T Value>
struct integral_constant : std::integral_constant<T, Value> {};

#define ECHO_DEFINE_BINARY_OPERATOR(SYMBOL)                       \
  template <class T1, T1 Value1, class T2, T2 Value2>             \
  auto operator SYMBOL(htl::integral_constant<T1, Value1>,        \
                       htl::integral_constant<T2, Value2>) {      \
    return htl::integral_constant<decltype(Value1 SYMBOL Value2), \
                                  (Value1 SYMBOL Value2)>{};      \
  }                                                               \
  template <class T1, T1 Value1, class T2, T2 Value2>             \
  auto operator SYMBOL(htl::integral_constant<T1, Value1>,        \
                       std::integral_constant<T2, Value2>) {      \
    return htl::integral_constant<decltype(Value1 SYMBOL Value2), \
                                  (Value1 SYMBOL Value2)>{};      \
  }                                                               \
  template <class T1, T1 Value1, class T2, T2 Value2>             \
  auto operator SYMBOL(std::integral_constant<T1, Value1>,        \
                       htl::integral_constant<T2, Value2>) {      \
    return htl::integral_constant<decltype(Value1 SYMBOL Value2), \
                                  (Value1 SYMBOL Value2)>{};      \
  }
ECHO_DEFINE_BINARY_OPERATOR(+)
ECHO_DEFINE_BINARY_OPERATOR(-)
ECHO_DEFINE_BINARY_OPERATOR(*)
ECHO_DEFINE_BINARY_OPERATOR(/ )
ECHO_DEFINE_BINARY_OPERATOR(% )
ECHO_DEFINE_BINARY_OPERATOR(< )
ECHO_DEFINE_BINARY_OPERATOR(> )
ECHO_DEFINE_BINARY_OPERATOR(<= )
ECHO_DEFINE_BINARY_OPERATOR(>= )
ECHO_DEFINE_BINARY_OPERATOR(== )
ECHO_DEFINE_BINARY_OPERATOR(!= )
ECHO_DEFINE_BINARY_OPERATOR(&&)
ECHO_DEFINE_BINARY_OPERATOR(|| )
#undef ECHO_DEFINE_BINARY_OPERATOR

template <class T, T Value, CONCEPT_REQUIRES(Value)>
auto operator||(htl::integral_constant<T, Value>,
                bool) -> htl::integral_constant<bool, true> {
  return {};
}

template <class T, T Value, CONCEPT_REQUIRES(Value)>
auto operator||(bool, htl::integral_constant<T, Value>)
    -> htl::integral_constant<bool, true> {
  return {};
}

template <class T, T Value, CONCEPT_REQUIRES(!Value)>
auto operator&&(htl::integral_constant<T, Value>,
                bool) -> htl::integral_constant<bool, false> {
  return {};
}

template <class T, T Value, CONCEPT_REQUIRES(!Value)>
auto operator&&(bool, htl::integral_constant<T, Value>)
    -> htl::integral_constant<bool, false> {
  return {};
}

#define ECHO_DEFINE_UNARY_OPERATOR(SYMBOL)                                   \
  template <class T, T Value>                                                \
  auto operator SYMBOL(htl::integral_constant<T, Value>) {                   \
    return htl::integral_constant<decltype(SYMBOL Value), (SYMBOL Value)>{}; \
  }
ECHO_DEFINE_UNARY_OPERATOR(-)
ECHO_DEFINE_UNARY_OPERATOR(+)
ECHO_DEFINE_UNARY_OPERATOR(!)
#undef ECHO_DEFINE_UNARY_OPERATOR

template <class T, T Value1, T Value2>
auto min(htl::integral_constant<T, Value1>, htl::integral_constant<T, Value2>)
    -> htl::integral_constant<T, (Value1 < Value2 ? Value1 : Value2)> {
  return {};
}

template <class T, T Value1>
T min(htl::integral_constant<T, Value1>, T y) {
  return std::min(Value1, y);
}

template <class T, T Value1>
T min(T x, htl::integral_constant<T, Value1>) {
  return std::min(x, Value1);
}

template <class T, T Value1, T Value2>
auto max(htl::integral_constant<T, Value1>, htl::integral_constant<T, Value2>)
    -> htl::integral_constant<T, (Value1 < Value2 ? Value2 : Value1)> {
  return {};
}

template <class T, T Value1>
T max(htl::integral_constant<T, Value1>, T y) {
  return std::max(Value1, y);
}

template <class T, T Value1>
T max(T x, htl::integral_constant<T, Value1>) {
  return std::max(x, Value1);
}
}
}
