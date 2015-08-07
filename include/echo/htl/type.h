#pragma once

#include <echo/htl/integral_constant.h>

namespace echo {
namespace htl {

//------------------------------------------------------------------------------
// Type
//------------------------------------------------------------------------------
template <class T>
struct Type {};

//------------------------------------------------------------------------------
// operator==
//------------------------------------------------------------------------------
template <class T1, class T2>
auto operator==(Type<T1>, Type<T2>)
    -> htl::integral_constant<bool, std::is_same<T1, T2>::value> {
  return {};
}

//------------------------------------------------------------------------------
// opeator!=
//------------------------------------------------------------------------------
template <class T1, class T2>
auto operator!=(Type<T1>, Type<T2>)
    -> htl::integral_constant<bool, !std::is_same<T1, T2>::value> {
  return {};
}
}
}
