#include <echo/htl/integral_constant.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

TEST_CASE("integral_constant") {
  htl::integral_constant<int, 3> x1;
  htl::integral_constant<short, 4> x2;

  auto x3 = x1+x2;
  auto x4 = x1 < x2;
  auto x5 = !x1;
  auto x6 = -x2;

  type_equal<decltype(x3), htl::integral_constant<int, 7>>();
  type_equal<decltype(x4), htl::integral_constant<bool, true>>();
  type_equal<decltype(x5), htl::integral_constant<bool, false>>();
  type_equal<decltype(x6), htl::integral_constant<int, -4>>();

  using T1 = decltype(false || htl::integral_constant<bool, true>());
  type_equal<T1, htl::integral_constant<bool, true>>();
}

TEST_CASE("integral_constant std") {
  htl::integral_constant<int, 3> x;
  std::integral_constant<int, 4> y;

  auto z = x + y;

  type_equal<decltype(z), htl::integral_constant<int, 7>>();
}
