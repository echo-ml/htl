#pragma once

#include <type_traits>

namespace echo {
namespace htl {

///////////////////////
// integral_constant //
///////////////////////

template <class T, T Value>
struct integral_constant : std::integral_constant<T, Value> {};

#define ECHO_DEFINE_BINARY_OPERATOR(SYMBOL)                    \
  template <class T1, T1 Value1, class T2, T2 Value2>          \
  auto operator SYMBOL(htl::integral_constant<T1, Value1>,     \
                       htl::integral_constant<T2, Value2>)     \
      ->htl::integral_constant<decltype(Value1 SYMBOL Value2), \
                               (Value1 SYMBOL Value2)> {       \
    return {};                                                 \
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

#define ECHO_DEFINE_UNARY_OPERATOR(SYMBOL)                               \
  template <class T, T Value>                                            \
  auto operator SYMBOL(htl::integral_constant<T, Value>)                 \
      ->htl::integral_constant<decltype(SYMBOL Value), (SYMBOL Value)> { \
    return {};                                                           \
  }
ECHO_DEFINE_UNARY_OPERATOR(-)
ECHO_DEFINE_UNARY_OPERATOR(!)
#undef ECHO_DEFINE_UNARY_OPERATOR
}
}
