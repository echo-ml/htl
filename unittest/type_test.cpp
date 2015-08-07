#include <echo/htl/type.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::htl;

TEST_CASE("type") {
  Type<int> t1, t2;
  Type<double> t3;
  type_equal<decltype(t1 == t2), htl::integral_constant<bool, true>>();
  type_equal<decltype(t1 == t3), htl::integral_constant<bool, false>>();
  type_equal<decltype(t1 != t2), htl::integral_constant<bool, false>>();
  type_equal<decltype(t1 != t3), htl::integral_constant<bool, true>>();
}
