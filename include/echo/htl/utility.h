#pragma once

#define DETAIL_NS detail_utility

#include <type_traits>
#include <utility>

namespace echo {
namespace htl {

////////////////////////
// copy_cv_qualifiers //
////////////////////////

namespace DETAIL_NS {
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

template <class A, class B>
using copy_cv_qualifiers =
    typename DETAIL_NS::copy_cv_qualifiers_impl<A, B>::type;
}
}

#undef DETAIL_NS
