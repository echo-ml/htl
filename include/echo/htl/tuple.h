#pragma once

#define DETAIL_NS detail_tuple

#include <echo/concept.h>
#include <echo/htl/pack.h>
#include <echo/htl/utility.h>
#include <echo/variadic_operator.h>

namespace echo {
namespace htl {

namespace DETAIL_NS {

//------------------------------------------------------------------------------
// tag
//------------------------------------------------------------------------------
template <int I>
struct tag {};

//------------------------------------------------------------------------------
// TupleBase
//------------------------------------------------------------------------------
template <class, class... Values>
struct TupleBase {};

template <std::size_t... Indexes, class... Values>
struct TupleBase<std::index_sequence<Indexes...>, Values...>
    : Pack<tag<Indexes>, Values>... {
  CONCEPT_MEMBER_REQUIRES(
      and_c<std::is_default_constructible<Values>::value...>())
  TupleBase() {}

  template <class... ValuesPrime,
            CONCEPT_REQUIRES((sizeof...(ValuesPrime) == sizeof...(Values)) &&
                             (sizeof...(Values) > 0))>
  explicit TupleBase(ValuesPrime&&... values_prime)
      : Pack<tag<Indexes>, Values>(std::forward<ValuesPrime>(values_prime))... {
  }
};
}

//------------------------------------------------------------------------------
// Tuple
//------------------------------------------------------------------------------
template <class... Values>
class Tuple : DETAIL_NS::TupleBase<std::make_index_sequence<sizeof...(Values)>,
                                   Values...> {
 public:
  using DETAIL_NS::TupleBase<std::make_index_sequence<sizeof...(Values)>,
                             Values...>::TupleBase;
  template <int I, CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
  decltype(auto) value() & {
    return unpack<DETAIL_NS::tag<I>>(*this);
  }
  template <int I, CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
  decltype(auto) value() const & {
    return unpack<DETAIL_NS::tag<I>>(*this);
  }
  template <int I, CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
  decltype(auto) value() && {
    return unpack<DETAIL_NS::tag<I>>(std::move(*this));
  }
};

//------------------------------------------------------------------------------
// make_tuple
//------------------------------------------------------------------------------
template <class... Values>
auto make_tuple(Values&&... values)
    -> decltype(Tuple<uncvref_t<Values>...>(std::forward<Values>(values)...)) {
  return Tuple<uncvref_t<Values>...>(std::forward<Values>(values)...);
}

namespace tuple_traits {

//------------------------------------------------------------------------------
// num_elements
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class>
struct num_elements_impl {};

template <class... Values>
struct num_elements_impl<Tuple<Values...>> {
  static constexpr int value = sizeof...(Values);
};
}

template <class T>
constexpr auto num_elements()
    -> decltype(DETAIL_NS::num_elements_impl<T>::value) {
  return DETAIL_NS::num_elements_impl<T>::value;
}

//------------------------------------------------------------------------------
// element_type
//------------------------------------------------------------------------------
namespace DETAIL_NS {
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

template <int I, class T>
using element_type = typename DETAIL_NS::element_type_impl<I, T>::type;
}

//------------------------------------------------------------------------------
// get
//------------------------------------------------------------------------------
template <int I, class... Values,
          CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
decltype(auto) get(Tuple<Values...>& tuple) {
  return tuple.template value<I>();
}

template <int I, class... Values,
          CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
decltype(auto) get(const Tuple<Values...>& tuple) {
  return tuple.template value<I>();
}

template <int I, class... Values,
          CONCEPT_REQUIRES((I >= 0) && (I < sizeof...(Values)))>
decltype(auto) get(Tuple<Values...>&& tuple) {
  return std::move(tuple).template value<I>();
}
}
}

#undef DETAIL_NS
