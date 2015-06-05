#include <echo/htl/algorithm.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

template <int I>
auto left(...) -> std::nullptr_t;

TEST_CASE("left") {
  Tuple<double, int, float> t1(3.0, 2, 1.5f);
  decltype(auto) t2 = left<1>(t1);
  CHECK(get<0>(t1) == 3.0);
  CHECK(get<1>(t1) == 2);
  CHECK(get<2>(t1) == 1.5f);

  using R1 = decltype(left<4>(t1));
  type_equal<R1, std::nullptr_t>();
}

TEST_CASE("map") {
  auto t1 = make_tuple(1, 2.0);
  auto f1 = [](auto x) { return x + 2; };
  auto t2 = map(f1, t1);

  type_equal<decltype(t2), Tuple<int, double>>();
  CHECK(get<0>(t2) == 3);
  CHECK(get<1>(t2) == 4.0);
}

TEST_CASE("remove_if") {
  auto t1 = make_tuple(3, 3.0, 5ul);
  auto pred = [](auto x) { return std::is_floating_point<decltype(x)>(); };
  auto t2 = remove_if(pred, t1);

  type_equal<decltype(t2), Tuple<int, unsigned long>>();
  CHECK(get<0>(t2) == 3);
  CHECK(get<1>(t2) == 5ul);

  int x = 3; 
  double y = 4;
  Tuple<int&, double&> t3(x, y);
  auto t4 = remove_if(pred, t3);

  type_equal<decltype(t4), Tuple<int&>>();
  x = 22;
  CHECK(get<0>(t4) == 22);
}

TEST_CASE("append") {
  auto t1 = make_tuple(3, 4.0);
  auto t2 = append(2ul, t1);

  type_equal<decltype(t2), Tuple<int, double, unsigned long>>();

  CHECK(get<0>(t2) == 3);
  CHECK(get<1>(t2) == 4.0);
  CHECK(get<2>(t2) == 2ul);
}

TEST_CASE("prepend") {
  auto t1 = make_tuple(3, 4.0);
  auto t2 = prepend(2ul, t1);

  type_equal<decltype(t2), Tuple<unsigned long, int, double>>();

  CHECK(get<0>(t2) == 2ul);
  CHECK(get<1>(t2) == 3);
  CHECK(get<2>(t2) == 4.0);
}
