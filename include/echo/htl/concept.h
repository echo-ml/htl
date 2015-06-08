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

//////////////////////
// boolean_constant //
//////////////////////

template <class T>
constexpr bool boolean_constant() {
  return std::is_convertible<T, std::true_type>::value ||
         std::is_convertible<T, std::false_type>::value;
}

//////////////
// mappable //
//////////////

namespace detail {
namespace concept {

template <class>
struct MappableImpl {};

template <std::size_t... Indexes>
struct MappableImpl<std::index_sequence<Indexes...>> : Concept {
  template <class Functor, class Tuple>
  auto require(Functor&& functor, Tuple&& tuple)
      -> list<valid<decltype(htl::make_tuple(
          functor(get<Indexes>(std::forward<Tuple>(tuple)))...))>()>;
};

struct Mappable : Concept {
  template <class Functor, class Tuple>
  auto require(Functor&& functor, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              models<MappableImpl<std::make_index_sequence<
                         tuple_traits::num_elements<uncvref_t<Tuple>>()>>,
                     Functor, Tuple>()>;
};
}
}

template <class Functor, class Tuple>
constexpr bool mappable() {
  return models<detail::concept::Mappable, Functor, Tuple>();
}

///////////////////////////////////
// applicable_constant_predicate //
///////////////////////////////////

namespace detail {
namespace concept {

template <class>
struct ApplicableConstantPredicateImpl {};

template <std::size_t... Indexes>
struct ApplicableConstantPredicateImpl<std::index_sequence<Indexes...>>
    : Concept {
  template <class Predicate, class Tuple>
  auto require(Predicate&& predicate, Tuple&& tuple) -> list<
      and_c<boolean_constant<decltype(predicate(get<Indexes>(tuple)))>()...>()>;
};

struct ApplicableConstantPredicate : Concept {
  template <class Predicate, class Tuple>
  auto require(Predicate&& predicate, Tuple&& tuple)
      -> list<htl::concept::tuple<uncvref_t<Tuple>>(),
              models<ApplicableConstantPredicateImpl<std::make_index_sequence<
                         tuple_traits::num_elements<uncvref_t<Tuple>>()>>,
                     Predicate, Tuple>()>;
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

namespace detail { namespace concept {

// struct LeftFoldable : Concept {
//   template<class Functor, class X0, class Tuple>
//   auto require(const Functor& functor, X0&& x0, Tuple&& tuple) -> list<
//     valid<
//       decltype(functor(x0, htl::get<0>(tuple)))
//     >()
//   >;
// };
//
// template<class Functor, class X0, class Tuple,
//   CONCEPT_REQUIRES(
//     tuple_traits::num_elements<uncvref_t<Tuple>>() == 0
//   )
// >
// constexpr bool left_foldable_impl() {
//   return true;
// }
//
// template<class Functor, class X0, class Tuple,
//   CONCEPT_REQUIRES(
//     tuple_traits::num_elements<uncvref_t<Tuple>>() != 0
//   )
// >
// constexpr bool left_foldable_impl() {
//   using Tail = decltype(htl::tail(std::declval<Tuple>()));
  // return models<LeftFoldable, Functor, X0, Tuple>() &&
  //   left_foldable_impl<;
//   return true;
// }

}}

template<class Functor, class X0, class Tuple>
constexpr bool left_foldable() {
  return false;
}

}
}
}
