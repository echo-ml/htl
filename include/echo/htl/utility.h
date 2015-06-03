#pragma once

#include <type_traits>
#include <utility>

namespace echo {
namespace htl {

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
