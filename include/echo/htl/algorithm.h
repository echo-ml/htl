#pragma once

#define DETAIL_NS detail_algorithm

#include <echo/htl/integral_constant.h>
#include <echo/htl/integer_sequence.h>
#include <echo/htl/tuple.h>
#include <echo/htl/concept.h>

namespace echo {
namespace htl {

//////////
// head //
//////////

template <class Tuple, CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
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

template <std::size_t... Indexes, class Tuple_,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple_>>()),
          CONCEPT_REQUIRES(and_c<(
              Indexes >= 0 &&
              Indexes < tuple_traits::num_elements<uncvref_t<Tuple_>>())...>())>
auto make_subtuple(htl::integer_sequence<std::size_t, Indexes...> indexes,
                   Tuple_&& tuple)
    -> decltype(
        Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
            htl::get<Indexes>(std::forward<Tuple_>(tuple))...)) {
  return Tuple<tuple_traits::element_type<Indexes, uncvref_t<Tuple_>>...>(
      htl::get<Indexes>(std::forward<Tuple_>(tuple))...);
}

////////////
// append //
////////////

namespace DETAIL_NS {
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

template <class Value, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>())>
auto append(Value&& value, Tuple&& tuple) -> decltype(DETAIL_NS::append_impl(
    std::make_index_sequence<tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
    std::forward<Value>(value), std::forward<Tuple>(tuple))) {
  return DETAIL_NS::append_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
      std::forward<Value>(value), std::forward<Tuple>(tuple));
}

/////////////
// prepend //
/////////////

namespace DETAIL_NS {
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

template <class Value, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>())>
auto prepend(Value&& value, Tuple&& tuple) -> decltype(DETAIL_NS::prepend_impl(
    std::make_index_sequence<tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
    std::forward<Value>(value), std::forward<Tuple>(tuple))) {
  return DETAIL_NS::prepend_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
      std::forward<Value>(value), std::forward<Tuple>(tuple));
}

//////////
// left //
//////////

namespace DETAIL_NS {
template <class, class>
struct left_impl {};

template <std::size_t... Indexes, class... Values>
struct left_impl<std::index_sequence<Indexes...>, Tuple<Values...>> {
  using type = Tuple<tuple_traits::element_type<Indexes, Tuple<Values...>>...>;
};

template <class Indexes, class Tuple>
using left_impl_t = typename left_impl<Indexes, Tuple>::type;
}

template <
    int N, class Tuple, CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
    CONCEPT_REQUIRES((N >= 0) &&
                     (N <= tuple_traits::num_elements<uncvref_t<Tuple>>()))>
auto left(Tuple&& tuple) -> copy_cv_qualifiers<
    Tuple,
    DETAIL_NS::left_impl_t<std::make_index_sequence<N>, uncvref_t<Tuple>>> {
  return reinterpret_cast<copy_cv_qualifiers<
      Tuple,
      DETAIL_NS::left_impl_t<std::make_index_sequence<N>, uncvref_t<Tuple>>>>(
      tuple);
}

///////////
// right //
///////////

namespace DETAIL_NS {

template <int, class, class>
struct right_impl {};

template <int I, std::size_t... Indexes, class... Values>
struct right_impl<I, std::index_sequence<Indexes...>, Tuple<Values...>> {
  using type =
      Tuple<tuple_traits::element_type<I + Indexes, Tuple<Values...>>...>;
};

template <class Indexes, class Tuple>
using right_impl_t =
    typename right_impl<tuple_traits::num_elements<Tuple>() - Indexes::size(),
                        Indexes, Tuple>::type;
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
    Tuple,
    DETAIL_NS::right_impl_t<std::make_index_sequence<N>, uncvref_t<Tuple>>> {
  return reinterpret_cast<copy_cv_qualifiers<
      Tuple,
      DETAIL_NS::right_impl_t<std::make_index_sequence<N>, uncvref_t<Tuple>>>>(
      htl::get<tuple_traits::num_elements<uncvref_t<Tuple>>() - N>(tuple));
}

///////////
// slice //
///////////

template <int I, int J, class Tuple,
          CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>()),
          CONCEPT_REQUIRES(I >= 0 &&
                           J <= tuple_traits::num_elements<uncvref_t<Tuple>>())>
decltype(auto) slice(Tuple&& tuple) {
  constexpr int N = tuple_traits::num_elements<uncvref_t<Tuple>>();
  return left<J - I>(right<N - I>(std::forward<Tuple>(tuple)));
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

namespace DETAIL_NS {
template <std::size_t I, class Functor, class... Tuples>
auto apply_impl(Functor&& functor, Tuples&&... tuples) {
  return std::forward<Functor>(functor)(
      htl::get<I>(std::forward<Tuples>(tuples))...);
}

template <std::size_t... Indexes, class Functor, class... Tuples>
auto map_impl(std::index_sequence<Indexes...>, Functor&& functor,
              Tuples&&... tuples) {
  return htl::Tuple<decltype(apply_impl<Indexes>(
      std::forward<Functor>(functor), std::forward<Tuples>(tuples)...))...>(
      apply_impl<Indexes>(std::forward<Functor>(functor),
                          std::forward<Tuples>(tuples)...)...);
}
}

template <
    class Functor, class TupleFirst, class... TuplesRest,
    CONCEPT_REQUIRES(concept::mappable<Functor, TupleFirst, TuplesRest...>())>
auto map(Functor&& functor, TupleFirst&& tuple_first,
         TuplesRest&&... tuples_rest) {
  return DETAIL_NS::map_impl(
      std::make_index_sequence<
          tuple_traits::num_elements<uncvref_t<TupleFirst>>()>(),
      std::forward<Functor>(functor), std::forward<TupleFirst>(tuple_first),
      std::forward<TuplesRest>(tuples_rest)...);
}

////////////////////////
// type_match_indexes //
////////////////////////

namespace DETAIL_NS {

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
      DETAIL_NS::type_match_indexes<std::true_type, BooleanTuple>;
  using FalseIndexes =
      DETAIL_NS::type_match_indexes<std::false_type, BooleanTuple>;
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
      DETAIL_NS::type_match_indexes<std::false_type, BooleanTuple>;
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

/////////////
// find_if //
/////////////

namespace DETAIL_NS {
template <class Index, class Predicate, class Tuple,
          CONCEPT_REQUIRES(tuple_traits::num_elements<uncvref_t<Tuple>>() == 0)>
auto find_if_impl(Index, const Predicate&, Tuple&&) {
  return htl::integral_constant<int, -1>{};
}

template <class Index, class Predicate, class Tuple,
          CONCEPT_REQUIRES(
              tuple_traits::num_elements<uncvref_t<Tuple>>() != 0 &&
              concept::boolean_true_constant<decltype(std::declval<Predicate>()(
                  htl::head(std::declval<Tuple>())))>())>
auto find_if_impl(Index i, const Predicate&, Tuple&&) {
  return i;
}

template <class Index, class Predicate, class Tuple,
          CONCEPT_REQUIRES(
              tuple_traits::num_elements<uncvref_t<Tuple>>() != 0 &&
              !concept::boolean_constant<decltype(std::declval<Predicate>()(
                  htl::head(std::declval<Tuple>())))>())>
int find_if_impl(Index, const Predicate& predicate, Tuple&& tuple);

template <
    class Index, class Predicate, class Tuple,
    CONCEPT_REQUIRES(concept::tuple<uncvref_t<Tuple>>() &&
                     tuple_traits::num_elements<uncvref_t<Tuple>>() != 0 &&
                     concept::applicable_predicate<Predicate, Tuple>() &&
                     concept::boolean_false_constant<decltype(std::declval<
                         Predicate>()(htl::head(std::declval<Tuple>())))>())>
auto find_if_impl(Index i, const Predicate& predicate, Tuple&& tuple) {
  return find_if_impl(i + htl::integral_constant<int, 1>(), predicate,
                      htl::tail(tuple));
}

template <class Index, class Predicate, class Tuple,
          CONCEPT_REQUIRES_REDECLARATION(
              tuple_traits::num_elements<uncvref_t<Tuple>>() != 0 &&
              !concept::boolean_constant<decltype(std::declval<Predicate>()(
                  htl::head(std::declval<Tuple>())))>())>
int find_if_impl(Index i, const Predicate& predicate, Tuple&& tuple) {
  if (predicate(htl::head(tuple)))
    return i;
  else
    return find_if_impl(i + 1, predicate, htl::tail(tuple));
}
}

template <class Predicate, class Tuple,
          CONCEPT_REQUIRES(concept::applicable_predicate<Predicate, Tuple>())>
auto find_if(const Predicate& predicate, Tuple&& tuple) {
  return DETAIL_NS::find_if_impl(htl::integral_constant<int, 0>(), predicate,
                                 tuple);
}

//////////////
// count_if //
//////////////

template <class Predicate, class Tuple,
          CONCEPT_REQUIRES(concept::applicable_predicate<Predicate, Tuple>() &&
                           tuple_traits::num_elements<uncvref_t<Tuple>>() == 0)>
auto count_if(const Predicate& predicate, Tuple&&) {
  return htl::integral_constant<int, 0>();
}

template <class Predicate, class Tuple,
          CONCEPT_REQUIRES(concept::applicable_predicate<Predicate, Tuple>() &&
                           tuple_traits::num_elements<uncvref_t<Tuple>>() != 0)>
auto count_if(const Predicate& predicate, Tuple&& tuple) {
  return (predicate(htl::head(tuple)) == htl::integral_constant<bool, true>()) +
         count_if(predicate, htl::tail(tuple));
}
}
}

#undef DETAIL_NS
