#pragma once

#define DETAIL_NS detail_integer_sequence

#include <echo/htl/tuple.h>
#include <echo/htl/integral_constant.h>
#include <utility>

namespace echo {
namespace htl {

//////////////////////
// integer_sequence //
//////////////////////

template <class Integer, Integer... Ix>
using integer_sequence = Tuple<htl::integral_constant<Integer, Ix>...>;

////////////////////
// index_sequence //
////////////////////

template <std::size_t... Ix>
using index_sequence = integer_sequence<std::size_t, Ix...>;

///////////////////////////
// make_integer_sequence //
///////////////////////////

namespace DETAIL_NS {
template <class Integer, Integer Offset, Integer... Ix>
auto make_integer_sequence_impl(std::integer_sequence<Integer, Ix...>) {
  return htl::integer_sequence<Integer, (Ix + Offset)...>();
}

template <class Integer, Integer N>
auto make_integer_sequence_impl(htl::integral_constant<Integer, N>) {
  return make_integer_sequence_impl<Integer, 0>(
      std::make_integer_sequence<Integer, N>());
}

template <class Integer, Integer A, Integer B>
auto make_integer_sequence_impl(htl::integral_constant<Integer, A>,
                                htl::integral_constant<Integer, B>) {
  return make_integer_sequence_impl<Integer, A>(
      std::make_integer_sequence<Integer, (B - A)>());
}
}

template <class Integer, Integer... Ix>
using make_integer_sequence = decltype(DETAIL_NS::make_integer_sequence_impl(
    htl::integral_constant<Integer, Ix>()...));

/////////////////////////
// make_index_sequence //
/////////////////////////

template <std::size_t... Ix>
using make_index_sequence = htl::make_integer_sequence<std::size_t, Ix...>;
}
}

#undef DETAIL_NS
