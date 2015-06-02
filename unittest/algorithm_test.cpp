#include <echo/htl/algorithm.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

template<int I>
auto left(...) -> std::nullptr_t;


TEST_CASE("algorithm") {
  Tuple<double, int, float> t1(3.0, 2, 1.5f);
  decltype(auto) t2 = left<1>(t1);
  CHECK(get<0>(t1) == 3.0);
  CHECK(get<1>(t1) == 2);
  CHECK(get<2>(t1) == 1.5f);

  using R1 = decltype(left<4>(t1));
  CHECK(std::is_same<R1, std::nullptr_t>::value);
}
