#pragma once

#include <echo/htl/tuple.h>
#include <echo/htl/functional.h>
#include <echo/htl/algorithm.h>
#include <echo/htl/integral_constant.h>

namespace echo {
namespace htl {

//////////////////////
// apply_comparison //
//////////////////////

template <class Predicate, class Lhs, class Rhs,
          CONCEPT_REQUIRES(
              concept::applicable_binary_predicate<Predicate, Lhs, Rhs>() &&
              tuple_traits::num_elements<uncvref_t<Lhs>>() == 0)>
auto apply_comparison(const Predicate& predicate, Lhs&& lhs, Rhs&& rhs) {
  return htl::integral_constant<bool, true>();
}

template <class Predicate, class Lhs, class Rhs,
          CONCEPT_REQUIRES(
              concept::applicable_binary_predicate<Predicate, Lhs, Rhs>() &&
              tuple_traits::num_elements<uncvref_t<Lhs>>() != 0)>
auto apply_comparison(const Predicate& predicate, Lhs&& lhs, Rhs&& rhs) {
  return predicate(htl::head(lhs), htl::head(rhs)) ||
         apply_comparison(predicate, tail(lhs), tail(rhs));
}

////////////////////
// tuple operator //
////////////////////

#define ECHO_MAKE_STRICT_OPERATOR(SYMBOL, COMPARISON)                         \
  template <class LhsTuple, class RhsTuple,                                   \
            CONCEPT_REQUIRES(                                                 \
                concept::applicable_binary_predicate<                         \
                    uncvref_t<decltype(COMPARISON)>, LhsTuple, RhsTuple>() && \
                tuple_traits::num_elements<uncvref_t<LhsTuple>>() == 1)>      \
  auto operator SYMBOL(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {          \
    return htl::head(lhs_tuple) SYMBOL htl::head(rhs_tuple);                  \
  }                                                                           \
  template <class LhsTuple, class RhsTuple,                                   \
            CONCEPT_REQUIRES(                                                 \
                concept::applicable_binary_predicate<                         \
                    uncvref_t<decltype(COMPARISON)>, LhsTuple, RhsTuple>() && \
                concept::applicable_binary_predicate<                         \
                    uncvref_t<decltype(htl::functional::equal)>, LhsTuple,    \
                    RhsTuple>() &&                                            \
                tuple_traits::num_elements<uncvref_t<LhsTuple>>() != 1)>      \
  auto operator SYMBOL(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {          \
    return htl::head(lhs_tuple) SYMBOL htl::head(rhs_tuple) ||                \
           (htl::head(lhs_tuple) == htl::head(rhs_tuple) &&                   \
            htl::tail(lhs_tuple) SYMBOL htl::tail(rhs_tuple));                \
  }
ECHO_MAKE_STRICT_OPERATOR(<, functional::less)
ECHO_MAKE_STRICT_OPERATOR(>, functional::greater)
#undef ECHO_MAKE_STRICT_OPERATOR

template <
    class LhsTuple, class RhsTuple,
    CONCEPT_REQUIRES(
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::greater)>, LhsTuple, RhsTuple>() &&
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::equal)>, LhsTuple, RhsTuple>())>
auto operator<=(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {
  return !(lhs_tuple > rhs_tuple);
}

template <
    class LhsTuple, class RhsTuple,
    CONCEPT_REQUIRES(
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::less)>, LhsTuple, RhsTuple>() &&
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::equal)>, LhsTuple, RhsTuple>())>
auto operator>=(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {
  return !(lhs_tuple > rhs_tuple);
}

template <
    class LhsTuple, class RhsTuple,
    CONCEPT_REQUIRES(
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::equal)>, LhsTuple, RhsTuple>() &&
        tuple_traits::num_elements<uncvref_t<LhsTuple>>() == 1)>
auto operator==(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {
  return htl::head(lhs_tuple) == htl::head(rhs_tuple);
}

template <
    class LhsTuple, class RhsTuple,
    CONCEPT_REQUIRES(
        concept::applicable_binary_predicate<
            uncvref_t<decltype(functional::equal)>, LhsTuple, RhsTuple>() &&
        tuple_traits::num_elements<uncvref_t<LhsTuple>>() != 1)>
auto operator==(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {
  return htl::head(lhs_tuple) == htl::head(rhs_tuple) &&
         htl::tail(lhs_tuple) == htl::tail(rhs_tuple);
}

template <class LhsTuple, class RhsTuple,
          CONCEPT_REQUIRES(concept::applicable_binary_predicate<
              uncvref_t<decltype(functional::equal)>, LhsTuple, RhsTuple>())>
auto operator!=(LhsTuple&& lhs_tuple, RhsTuple&& rhs_tuple) {
  return !(lhs_tuple == rhs_tuple);
}
}
}
