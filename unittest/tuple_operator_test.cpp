#include <echo/htl/tuple_operator.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::htl;

TEST_CASE("tuple_operator") {
  auto t1 = make_tuple(2, 3);
  auto t2 = make_tuple(3, 4);
  auto t3 = make_tuple(2.0, 1.0);
  auto t4 = htl::make_tuple(htl::integral_constant<int, 4>(), 3);
  auto t5 = htl::make_tuple(htl::integral_constant<int, 5>(), 2);
  auto t6 = htl::make_tuple(4, 3);

  auto x1 = t1 < t2;
  auto x2 = t1 > t2;
  auto x3 = t3 < t1;
  auto x4 = t4 < t5;
  auto x5 = t4 == t5;

  CHECK(x1);
  CHECK(!x2);
  CHECK(x3);
  CHECK(x4);
  type_equal<decltype(x4), htl::integral_constant<bool, true>>();

  CHECK((t6 == t4));
  CHECK(!(t5 == t4));
  CHECK(t5 != t4);
  type_equal<decltype(x5), htl::integral_constant<bool, false>>();
}
