#pragma once

#include <type_traits>
#include <utility>

namespace echo {
namespace htl {

///////////
// and_c //
///////////

namespace detail {
namespace utility {
template <bool... Values>
struct and_c_impl {
  static constexpr bool value = true;
};

template <bool ValueFirst, bool... ValuesRest>
struct and_c_impl<ValueFirst, ValuesRest...> {
  static constexpr bool value = ValueFirst && and_c_impl<ValuesRest...>::value;
};
}
}

template <bool... Values>
constexpr bool and_c() {
  return detail::utility::and_c_impl<Values...>::value;
}

////////////////////////
// copy_cv_qualifiers //
////////////////////////

namespace detail {
namespace utility {

template <class A, class B>
struct copy_cv_qualifiers_impl {
  using type = B;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<A&, B> {
  using type = B&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<const A&, B> {
  using type = const B&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<volatile A&, B> {
  using type = volatile B&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<const volatile A&, B> {
  using type = const volatile B&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<A&&, B> {
  using type = B&&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<const A&&, B> {
  using type = const B&&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<volatile A&&, B> {
  using type = volatile B&&;
};

template <class A, class B>
struct copy_cv_qualifiers_impl<const volatile A&&, B> {
  using type = const volatile B&&;
};
}
}

template <class A, class B>
using copy_cv_qualifiers =
    typename detail::utility::copy_cv_qualifiers_impl<A, B>::type;
}
}
