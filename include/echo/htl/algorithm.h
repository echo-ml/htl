#pragma once

#include <echo/htl/tuple.h>
#include <echo/htl/concept.h>

namespace echo {
namespace htl {

///////////////////
// make_subtuple //
///////////////////

namespace detail {
namespace algorithm {

template <std::size_t... Indexes, class Tuple>
auto make_subtuple_impl(std::index_sequence<Indexes...>, Tuple&& tuple)
    -> decltype(htl::make_tuple(get<Indexes>(std::forward<Tuple>(tuple))...)) {
  return htl::make_tuple(htl::get<Indexes>(std::forward<Tuple>(tuple))...);
}
}
}

template <std::size_t... Indexes, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
          CONCEPT_REQUIRES(and_c<
              (Indexes >= 0 &&
               Indexes < tuple_traits::num_elements<uncvref_t<Tuple>>())...>())>
auto make_subtuple(std::index_sequence<Indexes...> indexes, Tuple&& tuple)
    -> decltype(detail::algorithm::make_subtuple_impl(
        indexes, std::forward<Tuple>(tuple))) {
  return detail::algorithm::make_subtuple_impl(indexes,
                                               std::forward<Tuple>(tuple));
}

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

/////////
// map //
/////////

namespace detail {
namespace algorithm {

template <std::size_t... Indexes, class Functor, class Tuple>
auto map_impl(std::index_sequence<Indexes...>, Functor&& functor,
              Tuple&& tuple) {
  return htl::make_tuple(functor(get<Indexes>(std::forward<Tuple>(tuple)))...);
}
}
}

template <class Functor, class Tuple,
          CONCEPT_REQUIRES(concept::mappable<Functor, Tuple>())>
auto map(Functor&& functor, Tuple&& tuple) {
  return detail::algorithm::map_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
      std::forward<Functor>(functor), std::forward<Tuple>(tuple));
}

////////////////////////
// type_match_indexes //
////////////////////////

namespace detail {
namespace algorithm {

template <class, int, class, class Result>
struct type_match_indexes_impl {
  using type = Result;
};

template <class T, int I, class BFirst, class... BRest, std::size_t... Indexes>
struct type_match_indexes_impl<T, I, Tuple<BFirst, BRest...>,
                               std::index_sequence<Indexes...>> {
  using type = std::conditional_t<
      std::is_convertible<BFirst, T>::value,
      typename type_match_indexes_impl<
          T, I + 1, Tuple<BRest...>, std::index_sequence<Indexes..., I>>::type,
      typename type_match_indexes_impl<T, I + 1, Tuple<BRest...>,
                                       std::index_sequence<Indexes...>>::type>;
};

template <class T, class BooleanTuple>
using type_match_indexes =
    typename type_match_indexes_impl<T, 0, BooleanTuple,
                                     std::index_sequence<>>::type;
}
}

///////////////
// partition //
///////////////

template <class Predicate, class Tuple,
          CONCEPT_REQUIRES(concept::applicable_predicate<Predicate, Tuple>() &&
                           std::is_copy_constructible<uncvref_t<Tuple>>::value)>
auto partition(const Predicate& predicate, Tuple&& tuple) {
  using BooleanTuple = decltype(map(predicate, tuple));
  using TrueIndexes =
      detail::algorithm::type_match_indexes<std::true_type, BooleanTuple>;
  using FalseIndexes =
      detail::algorithm::type_match_indexes<std::false_type, BooleanTuple>;
  return make_tuple(make_subtuple(TrueIndexes(), std::forward<Tuple>(tuple)),
                    make_subtuple(FalseIndexes(), std::forward<Tuple>(tuple)));
}

///////////////
// remove_if //
///////////////

template <class Predicate, class Tuple,
          CONCEPT_REQUIRES(concept::applicable_predicate<Predicate, Tuple>() &&
                           std::is_copy_constructible<uncvref_t<Tuple>>::value)>
auto remove_if(const Predicate& predicate, Tuple&& tuple) {
  using BooleanTuple = decltype(map(predicate, tuple));
  using FalseIndexes =
      detail::algorithm::type_match_indexes<std::false_type, BooleanTuple>;
  return make_subtuple(FalseIndexes(), std::forward<Tuple>(tuple));
}
}
}
