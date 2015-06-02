#pragma once

#include <echo/htl/tuple.h>

namespace echo {
namespace htl {

//////////
// left //
//////////

namespace detail {
namespace algorithm {

template <class, class>
struct left_impl {};

template <std::size_t... Indexes, class... Values>
struct left_impl<std::index_sequence<Indexes...>, Tuple<Values...>> {
  using type = Tuple<tuple_traits::element_type<Indexes, Tuple<Values...>>...>;
};
}
}

template <int I, class T,
          std::enable_if_t<concept::tuple<std::decay_t<T>>(), int> = 0,
          std::enable_if_t<
              (I >= 0) && (I < tuple_traits::num_elements<std::decay_t<T>>()),
              int> = 0>
auto left(T&& t) -> copy_cv_qualifiers<
    T, detail::algorithm::left_impl<std::make_index_sequence<I>,
                                    std::decay_t<T>>> {
  return reinterpret_cast<
      copy_cv_qualifiers<T, detail::algorithm::left_impl<
                                std::make_index_sequence<I>, std::decay_t<T>>>>(
      t);
}
}
}
