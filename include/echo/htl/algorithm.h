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

template <int I, class T, CONCEPT_REQUIRES(concept::tuple<uncvref_t<T>>()),
          CONCEPT_REQUIRES((I >= 0) &&
                           (I < tuple_traits::num_elements<uncvref_t<T>>()))>
auto left(T&& t) -> copy_cv_qualifiers<
    T,
    detail::algorithm::left_impl<std::make_index_sequence<I>, uncvref_t<T>>> {
  return reinterpret_cast<copy_cv_qualifiers<
      T,
      detail::algorithm::left_impl<std::make_index_sequence<I>, uncvref_t<T>>>>(
      t);
}

///////////////
// remove_if //
///////////////

namespace detail {
namespace algorithm {

namespace concept {

template <class>
struct ApplicablePredicate {};

template <std::size_t... Indexes>
struct ApplicablePredicate<std::index_sequence<Indexes...>> : Concept {
  template <class Tuple, class Predicate>
  auto require(Tuple&& tuple, Predicate&& predicate) -> list<
      and_c<std::is_convertible<decltype(predicate(get<Indexes>(tuple))),
                                std::true_type>::value ||
            std::is_convertible<decltype(predicate(get<Indexes>(tuple))),
                                std::false_type>::value...>()>;
};

template <class Tuple, class Predicate>
constexpr bool applicable_predicate() {
  return true;
}
}
}
}

template <class T, class Predicate,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<T>>())>
auto remove_if(const Predicate& predicate, T&& tuple) {}
}
}
