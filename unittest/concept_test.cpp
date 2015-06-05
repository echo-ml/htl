#include <echo/htl/concept.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

TEST_CASE("applicable_predicate") {
  using Tuple1 = Tuple<int, double>;

  auto pred1 = [](auto x) {
    return std::is_integral<decltype(x)>();
  };

  CHECK(htl::concept::applicable_predicate<decltype(pred1), Tuple1>());
  CHECK(!htl::concept::applicable_predicate<int, Tuple1>());
}

struct MyStruct {};

TEST_CASE("mappable") {
  using Tuple1 = Tuple<int, double>;
  using Tuple2 = Tuple<int, MyStruct>;

  auto f = [](auto x) -> decltype(x+3) { return x+3; };

  CHECK(htl::concept::mappable<decltype(f), Tuple1>());
  CHECK(!htl::concept::mappable<decltype(f), Tuple2>());
}
