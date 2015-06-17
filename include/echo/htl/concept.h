#pragma once

#include <echo/htl/tuple.h>

namespace echo {
namespace htl {
namespace concept {

///////////
// tuple //
///////////

namespace detail {
namespace tuple {

template <class>
struct tuple_impl {
  static constexpr bool value = false;
};

template <class... Values>
struct tuple_impl<Tuple<Values...>> {
  static constexpr bool value = true;
};
}
}

template <class T>
constexpr bool tuple() {
  return detail::tuple::tuple_impl<T>::value;
}

/////////////
// boolean //
/////////////

template <class T>
constexpr bool boolean() {
  return std::is_convertible<T, bool>::value;
}

///////////////////////////
// boolean_true_constant //
///////////////////////////

namespace detail {
namespace concept {
struct BooleanTrueConstant : Concept {
  template <class T>
  auto require(T && ) -> list<boolean<T>(), static_cast<bool>(T::value)>;
};
}
}

template <class T>
constexpr bool boolean_true_constant() {
  return models<detail::concept::BooleanTrueConstant, T>();
}

////////////////////////////
// boolean_false_constant //
////////////////////////////

namespace detail {
namespace concept {
struct BooleanFalseConstant : Concept {
  template <class T>
  auto require(T && ) -> list<boolean<T>(), static_cast<bool>(!T::value)>;
};
}
}

template <class T>
constexpr bool boolean_false_constant() {
  return models<detail::concept::BooleanFalseConstant, T>();
}

//////////////////////
// boolean_constant //
//////////////////////

template <class T>
constexpr bool boolean_constant() {
  return boolean_true_constant<T>() || boolean_false_constant<T>();
}

//////////////
// mappable //
//////////////

namespace detail {
namespace concept {

template <std::size_t Index, class Functor, class... Tuples>
auto apply_map_element(Functor&& functor, Tuples&&... tuples)
    -> decltype(std::forward<Functor>(functor)(
        htl::get<Index>(std::forward<Tuples>(tuples))...));

template<std::size_t Index, class Functor, class... Tuples>
using apply_map_element_type = decltype(
  apply_map_element<Index>(std::declval<Functor>(), std::declval<Tuples>()...)
);

template<std::size_t... Indexes, class Functor, class... Tuples>
auto apply_map(std::index_sequence<Indexes...>, Functor&& functor,
  Tuples&&... tuples) 
  -> decltype(
    htl::Tuple
    // <
    //   apply_map_element_type<Indexes, Functor, Tuples...>...
    // >
    <
    decltype(
      apply_map_element<Indexes>(std::forward<Functor>(functor),
      std::forward<Tuples>(tuples)...)
      )...
    >
    (
      apply_map_element<Indexes>(std::forward<Functor>(functor),
      std::forward<Tuples>(tuples)...)...
      )
  );

// template <std::size_t... Indexes, class Functor, class Tuple>
// auto apply_map(std::index_sequence<Indexes...>, Functor&& functor,
//                Tuple&& tuple)
//     -> decltype(htl::make_tuple(functor(get<Indexes>(tuple))...));
//
// struct Mappable : Concept {
//   template <class Functor, class Tuple>
//   auto require(Functor&& functor, Tuple&& tuple)
//       -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
//               htl::concept::tuple<decltype(apply_map(
//                   std::make_index_sequence<
//                       tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
//                   functor, tuple))>()>;
// };

struct Mappable : Concept {
  template<class Functor, class TupleFirst, class... TuplesRest>
  auto require(Functor&& functor, TupleFirst&& tuple_first, TuplesRest&&... tuples_rest) ->
    list<
      htl::concept::tuple<uncvref_t<TupleFirst>>()
      ,
      htl::concept::tuple<decltype(
        apply_map(
          std::make_index_sequence<tuple_traits::num_elements<uncvref_t<TupleFirst>>()>(),
            std::forward<Functor>(functor), 
            std::forward<TupleFirst>(tuple_first),
            std::forward<TuplesRest>(tuples_rest)...)
      )>()
    >;
};
}
}

template <class Functor, class Tuple>
constexpr bool mappable() {
  return models<detail::concept::Mappable, Functor, Tuple>();
}

//////////////////////////
// applicable_predicate //
//////////////////////////

namespace detail {
namespace concept {

template <class>
struct ApplicablePredicateImpl {};

template <std::size_t... Indexes>
struct ApplicablePredicateImpl<std::index_sequence<Indexes...>> : Concept {
  template <class Predicate, class Tuple>
  auto require(Predicate&& predicate, Tuple&& tuple) -> list<
      and_c<boolean<decltype(predicate(htl::get<Indexes>(tuple)))>()...>()>;
};

struct ApplicablePredicate : Concept {
  template <class Predicate, class Tuple>
  auto require(Predicate&& predicate, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              models<ApplicablePredicateImpl<std::make_index_sequence<
                         tuple_traits::num_elements<uncvref_t<Tuple>>()>>,
                     Predicate, Tuple>()>;
};
}
}

template <class Predicate, class Tuple>
constexpr bool applicable_predicate() {
  return models<detail::concept::ApplicablePredicate, Predicate, Tuple>();
}

/////////////////////////////////
// applicable_binary_predicate //
/////////////////////////////////

namespace detail {
namespace concept {

template <std::size_t... Indexes, class Predicate, class TupleLhs,
          class TupleRhs>
auto applicable_binary_predicate_impl(std::index_sequence<Indexes...>,
                                      Predicate&& predicate,
                                      TupleLhs&& tuple_lhs,
                                      TupleRhs&& tuple_rhs)
    -> std::integral_constant<bool,
                              and_c<boolean<decltype(predicate(
                                  htl::get<Indexes>(tuple_lhs),
                                  htl::get<Indexes>(tuple_rhs)))>()...>()>;

inline auto applicable_binary_predicate_impl(...) -> std::false_type;

struct ApplicableBinaryPredicate : Concept {
  template <class Predicate, class TupleLhs, class TupleRhs>
  auto require(Predicate&& predicate, TupleLhs&& tuple_lhs,
               TupleRhs&& tuple_rhs)
      -> list<htl::concept::tuple<uncvref_t<TupleLhs>>(),
              htl::concept::tuple<uncvref_t<TupleRhs>>(),
              tuple_traits::num_elements<uncvref_t<TupleLhs>>() ==
                  tuple_traits::num_elements<uncvref_t<TupleRhs>>(),
              boolean_true_constant<decltype(applicable_binary_predicate_impl(
                  std::make_index_sequence<
                      tuple_traits::num_elements<uncvref_t<TupleLhs>>()>(),
                  predicate, tuple_lhs, tuple_rhs))>()>;
};
}
}

template <class Predicate, class TupleLhs, class TupleRhs>
constexpr bool applicable_binary_predicate() {
  return models<detail::concept::ApplicableBinaryPredicate, Predicate, TupleLhs,
                TupleRhs>();
}

///////////////////////////////////
// applicable_constant_predicate //
///////////////////////////////////

namespace detail {
namespace concept {

template <std::size_t... Indexes, class Predicate, class Tuple>
auto applicable_constant_predicate_impl(std::index_sequence<Indexes...>,
                                        Predicate&& predicate, Tuple&& tuple)
    -> std::integral_constant<bool, and_c<boolean_constant<decltype(predicate(
                                        htl::get<Indexes>(tuple)))>()...>()>;

inline auto applicable_constant_predicate_impl(...) -> std::false_type;

struct ApplicableConstantPredicate : Concept {
  template <class Predicate, class Tuple>
  auto require(Predicate&& predicate, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              boolean_true_constant<decltype(applicable_constant_predicate_impl(
                  std::make_index_sequence<
                      tuple_traits::num_elements<uncvref_t<Tuple>>()>(),
                  predicate, tuple))>()>;
};
}
}

template <class Predicate, class Tuple>
constexpr bool applicable_constant_predicate() {
  return models<detail::concept::ApplicableConstantPredicate, Predicate,
                Tuple>();
}

///////////////////
// left_foldable //
///////////////////

namespace detail {
namespace concept {

template <int I, int N>
struct LeftFoldableImpl : Concept {
  template <class Functor, class X, class Tuple>
  auto require(const Functor& functor, X&& x, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              models<LeftFoldableImpl<I + 1, N>, Functor,
                     decltype(functor(std::forward<X>(x),
                                      htl::get<I>(std::forward<Tuple>(tuple)))),
                     Tuple>()>;
};

template <int N>
struct LeftFoldableImpl<N, N> : Concept {
  template <class Functor, class X, class Tuple>
  auto require(const Functor&, X&&, Tuple && )
      -> list<htl::concept::tuple<uncvref_t<Tuple>>()>;
};

struct LeftFoldable : Concept {
  template <class Functor, class X0, class Tuple>
  auto require(const Functor& functor, X0&& x0, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              models<LeftFoldableImpl<
                         0, tuple_traits::num_elements<uncvref_t<Tuple>>()>,
                     Functor, X0, Tuple>()>;
};
}
}

template <class Functor, class X0, class Tuple>
constexpr bool left_foldable() {
  return models<detail::concept::LeftFoldable, Functor, X0, Tuple>();
}
}
}
}
