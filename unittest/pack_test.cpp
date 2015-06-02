#include <echo/htl/pack.h>
#include <echo/test.h>
#include <utility>

using namespace echo;
using namespace echo::htl;

using Empty1 = std::true_type;

struct A1 : Pack<Empty1> {
  int x;
};

struct A2 : Pack<int> {
  A2() : Pack<int>(7) {}
  int x;
};

struct tag1 {};
struct tag2 {};

struct A3 : Pack<tag1, int>, Pack<tag2, double> {
  A3(int x, double y) : Pack<tag1, int>(x), Pack<tag2, double>(y) {}
};

TEST_CASE("pack") {
  CHECK(sizeof(A1) == sizeof(int));
  CHECK(sizeof(A2) == sizeof(int)*2);

  A1 a1;
  A2 a2;
  A3 a3(1, 2);
  CHECK(unpack(a1) == true);
  CHECK(unpack(a2) == 7);
  CHECK(unpack<int>(a2) == 7);

  CHECK(unpack<tag1>(a3) == 1);
  CHECK(unpack<tag2>(a3) == 2);

  // CHECK(unpack(tag1(), a3) == 1);
  // CHECK(unpack(tag2(), a3) == 2);
}
