#pragma once

#include <echo/htl/tuple.h>
#include <echo/htl/concept.h>

namespace echo {
namespace htl {

//////////
// head //
//////////

template<class Tuple, 
  CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
  CONCEPT_REQUIRES(tuple_traits::num_elements<uncvref_t<Tuple>>() > 0)>
decltype(auto) head(Tuple&& tuple) {
  return htl::get<0>(std::forward<Tuple>(tuple));
}
  

///////////////////
// make_subtuple //
///////////////////

template <std::size_t... Indexes, class Tuple_,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple_>>()),
          CONCEPT_REQUIRES(and_c<(
              Indexes >= 0 &&
              Indexes < tuple_traits::num_elements<uncvref_t<Tuple_>>())...>())>
auto make_subtuple(std::index_sequence<Indexes...> indexes, Tuple_&& tuple)
    -> decltype(
        Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
            htl::get<Indexes>(std::forward<Tuple_>(tuple))...)) {
  return Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
      htl::get<Indexes>(std::forward<Tuple_>(tuple))...);
}

////////////
// append //
////////////

namespace detail {
namespace algorithm {
template <std::size_t... Indexes, class Value, class Tuple_>
auto append_impl(std::index_sequence<Indexes...>, Value&& value, Tuple_&& tuple)
    -> decltype(Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...,
                      uncvref_t<Value>>(
        htl::get<Indexes>(std::forward<Tuple_>(tuple))...,
        std::forward<Value>(value))) {
  return Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...,
               uncvref_t<Value>>(
      htl::get<Indexes>(std::forward<Tuple_>(tuple))...,
      std::forward<Value>(value));
}
}
}

template <class Value, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>())>
auto append(Value&& value, Tuple&& tuple)
    -> decltype(detail::algorithm::append_impl(
        std::make_index_sequence<
            tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
        std::forward<Value>(value), std::forward<Tuple>(tuple))) {
  return detail::algorithm::append_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
      std::forward<Value>(value), std::forward<Tuple>(tuple));
}

/////////////
// prepend //
/////////////

namespace detail {
namespace algorithm {
template <std::size_t... Indexes, class Value, class Tuple_>
auto prepend_impl(std::index_sequence<Indexes...>, Value&& value,
                  Tuple_&& tuple)
    -> decltype(
        Tuple<uncvref_t<Value>,
              tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
            std::forward<Value>(value),
            htl::get<Indexes>(std::forward<Tuple_>(tuple))...)) {
  return Tuple<uncvref_t<Value>,
               tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
      std::forward<Value>(value),
      htl::get<Indexes>(std::forward<Tuple_>(tuple))...);
}
}
}

template <class Value, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>())>
auto prepend(Value&& value, Tuple&& tuple)
    -> decltype(detail::algorithm::prepend_impl(
        std::make_index_sequence<
            tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
        std::forward<Value>(value), std::forward<Tuple>(tuple))) {
  return detail::algorithm::prepend_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
      std::forward<Value>(value), std::forward<Tuple>(tuple));
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

template <class Indexes, class Tuple>
using left_impl_t = typename left_impl<Indexes, Tuple>::type;
}
}

template <
    int N, class Tuple, CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
    CONCEPT_REQUIRES((N >= 0) &&
                     (N <= tuple_traits::num_elements<uncvref_t<Tuple>>()))>
auto left(Tuple&& tuple) -> copy_cv_qualifiers<
    Tuple, detail::algorithm::left_impl_t<std::make_index_sequence<N>,
                                          uncvref_t<Tuple>>> {
  return reinterpret_cast<copy_cv_qualifiers<
      Tuple, detail::algorithm::left_impl_t<std::make_index_sequence<N>,
                                            uncvref_t<Tuple>>>>(tuple);
}

///////////
// right //
///////////

namespace detail {
namespace algorithm {

template <int, class, class>
struct right_impl {};

template <int I, std::size_t... Indexes, class... Values>
struct right_impl<I, std::index_sequence<Indexes...>, Tuple<Values...>> {
  using type = Tuple<tuple_traits::element_type<
      I + Indexes, Tuple<Values...>>...>;
};

template <class Indexes, class Tuple>
using right_impl_t = typename right_impl<
  tuple_traits::num_elements<Tuple>() - Indexes::size(),
  Indexes, Tuple>::type;
}
}

template <int N, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
          CONCEPT_REQUIRES(N == 0)>
auto right(Tuple&& tuple) -> copy_cv_qualifiers<Tuple, htl::Tuple<>> {
  return reinterpret_cast<copy_cv_qualifiers<Tuple, htl::Tuple<>>>(tuple);
}

template <int N, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
          CONCEPT_REQUIRES(
              (N > 0) && (N <= tuple_traits::num_elements<uncvref_t<Tuple>>()))>
auto right(Tuple&& tuple) -> copy_cv_qualifiers<
    Tuple, detail::algorithm::right_impl_t<std::make_index_sequence<N>,
                                           uncvref_t<Tuple>>> {
  return reinterpret_cast<copy_cv_qualifiers<
      Tuple, detail::algorithm::right_impl_t<std::make_index_sequence<N>,
                                             uncvref_t<Tuple>>>>(
      htl::get<tuple_traits::num_elements<uncvref_t<Tuple>>() - N>(tuple));
}

//////////
// tail //
//////////

template <class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>() &&
                           tuple_traits::num_elements<uncvref_t<Tuple>>() > 0)>
decltype(auto) tail(Tuple&& tuple) {
  return right<tuple_traits::num_elements<uncvref_t<Tuple>>() - 1>(
      std::forward<Tuple>(tuple));
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
          CONCEPT_REQUIRES(
              concept::applicable_constant_predicate<Predicate, Tuple>() &&
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
          CONCEPT_REQUIRES(
              concept::applicable_constant_predicate<Predicate, Tuple>() &&
              std::is_copy_constructible<uncvref_t<Tuple>>::value)>
auto remove_if(const Predicate& predicate, Tuple&& tuple) {
  using BooleanTuple = decltype(map(predicate, tuple));
  using FalseIndexes =
      detail::algorithm::type_match_indexes<std::false_type, BooleanTuple>;
  return make_subtuple(FalseIndexes(), std::forward<Tuple>(tuple));
}

///////////////
// left_fold //
///////////////

template <class Functor, class X0, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>() &&
                           tuple_traits::num_elements<uncvref_t<Tuple>>() ==
                               0 &&
                           std::is_copy_constructible<uncvref_t<X0>>::value)>
auto left_fold(const Functor& functor, X0&& x0, Tuple&& tuple) {
  return x0;
}

template <class Functor, class X0, class Tuple,
          CONCEPT_REQUIRES(concept::left_foldable<Functor, X0, Tuple>() &&
                           tuple_traits::num_elements<uncvref_t<Tuple>>() > 0)>
auto left_fold(const Functor& functor, X0&& x0, Tuple&& tuple) {
  return left_fold(functor, functor(std::forward<X0>(x0),
                                    htl::get<0>(std::forward<Tuple>(tuple))),
                   tail(std::forward<Tuple>(tuple)));
}
}
}
