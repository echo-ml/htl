#pragma once

#include <echo/htl/pack.h>
#include <echo/htl/utility.h>

namespace echo {
namespace htl {

namespace detail {
namespace tuple {

/////////
// tag //
/////////

template <int I>
struct tag {};

///////////////
// TupleBase //
///////////////

template <class, class... Values>
struct TupleBase {};

template <std::size_t... Indexes, class... Values>
struct TupleBase<std::index_sequence<Indexes...>, Values...>
    : Pack<tag<Indexes>, Values>... {
  template <bool X = true,
            std::enable_if_t<
                X && and_c<std::is_default_constructible<Values>::value...>(),
                int> = 0>
  TupleBase() {}

  template <class... ValuesPrime,
            std::enable_if_t<(sizeof...(ValuesPrime) == sizeof...(Values)) &&
                                 (sizeof...(Values) > 0),
                             int> = 0>
  // below check is broken on intel compiler
  // std::enable_if_t<and_c<std::is_constructible<Values,
  //                                              ValuesPrime>::value...>(),
  //                  int> = 0>
  TupleBase(ValuesPrime&&... values_prime)
      : Pack<tag<Indexes>, Values>(std::forward<ValuesPrime>(values_prime))... {
  }
};
}
}

///////////
// Tuple //
///////////

template <class... Values>
class Tuple
    : detail::tuple::TupleBase<std::make_index_sequence<sizeof...(Values)>,
                               Values...> {
 public:
  using detail::tuple::TupleBase<std::make_index_sequence<sizeof...(Values)>,
                                 Values...>::TupleBase;
  template <int I,
            std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>
  decltype(auto) value() & {
    return unpack<detail::tuple::tag<I>>(*this);
  }
  template <int I,
            std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>
  decltype(auto) value() const & {
    return unpack<detail::tuple::tag<I>>(*this);
  }
  template <int I,
            std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>
  decltype(auto) value() && {
    return unpack<detail::tuple::tag<I>>(std::move(*this));
  }
};

////////////////
// make_tuple //
////////////////

template <class... Values>
auto make_tuple(Values&&... values)
    -> decltype(Tuple<std::decay_t<Values>...>(values...)) {
  return Tuple<std::decay_t<Values>...>(values...);
}

///////////
// tuple //
///////////

namespace concept {
namespace detail {
namespace tuple {

template<class>
struct tuple_impl { static constexpr bool value = false; };

template<class... Values>
struct tuple_impl<Tuple<Values...>> { static constexpr bool value = true; };
}
}

template <class T>
constexpr bool tuple() {
  return detail::tuple::tuple_impl<T>::value;
}
}

namespace tuple_traits {

//////////////////
// num_elements //
//////////////////

namespace detail {
namespace tuple {

template <class>
struct num_elements_impl {};

template <class... Values>
struct num_elements_impl<Tuple<Values...>> {
  static constexpr int value = sizeof...(Values);
};
}
}

template <class T>
constexpr auto num_elements()
    -> decltype(detail::tuple::num_elements_impl<T>::value) {
  return detail::tuple::num_elements_impl<T>::value;
}

//////////////////
// element_type //
//////////////////

namespace detail {
namespace tuple {

template <int, class>
struct element_type_impl {};

template <int I, class ValueFirst, class... ValuesRest>
struct element_type_impl<I, Tuple<ValueFirst, ValuesRest...>> {
  using type = typename element_type_impl<I - 1, Tuple<ValuesRest...>>::type;
};

template <class ValueFirst, class... ValuesRest>
struct element_type_impl<0, Tuple<ValueFirst, ValuesRest...>> {
  using type = ValueFirst;
};
}
}

template <int I, class T>
using element_type = typename detail::tuple::element_type_impl<I, T>::type;
}

/////////
// get //
/////////

template <int I, class... Values,
          std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>
decltype(auto) get(Tuple<Values...>& tuple) {
  return tuple.template value<I>();
}
template <int I, class... Values,
          std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>

decltype(auto) get(const Tuple<Values...>& tuple) {
  return tuple.template value<I>();
}
template <int I, class... Values,
          std::enable_if_t<(I >= 0) && (I < sizeof...(Values)), int> = 0>

decltype(auto) get(Tuple<Values...>&& tuple) {
  return std::move(tuple).template value<I>();
}
}
}
