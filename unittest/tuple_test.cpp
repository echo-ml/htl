#include <echo/htl/tuple.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

TEST_CASE("tuple") {
  Tuple<double, int> t1, t2(3.0, 7), t3(7, 8);

  SECTION("empty type") {
    Tuple<std::true_type, int> t4;
    CHECK(sizeof(t4) == sizeof(int));
  }

  SECTION("sequential memory") {
    Tuple<int, int> t5(1, 2);
    int* p = reinterpret_cast<int*>(&t5);
    CHECK(p[0] == 1);
    CHECK(p[1] == 2);
  }

  SECTION("get") {
    Tuple<int, double> t(1, 2.0);
    CHECK(get<0>(t) == 1);
    CHECK(get<1>(t) == 2.0);
  }

  SECTION("reference") {
    Tuple<int, double, std::true_type> t(1, 2.0, std::true_type{});
    const auto& ctref = t;
    using R1 = decltype(get<0>(t));
    using R2 = decltype(get<0>(ctref));
    using R3 = decltype(get<2>(t));
    using R4 = decltype(get<0>(std::move(t)));
    using R5 = decltype(get<2>(std::move(t)));
    using R6 = decltype(get<2>(ctref));
    CHECK(std::is_same<R1, int&>::value);
    CHECK(std::is_same<R2, const int&>::value);
    CHECK(std::is_same<R3, std::true_type&>::value);
    CHECK(std::is_same<R4, int&&>::value);
    CHECK(std::is_same<R5, std::true_type&&>::value);
    CHECK(std::is_same<R6, const std::true_type&>::value);
  }

  SECTION("traits") {
    using T1 = Tuple<int, double>;
    CHECK(tuple_traits::num_elements<T1>() == 2);
    CHECK(std::is_same<tuple_traits::element_type<0, T1>, int>::value);
  }

  SECTION("make_tuple") {
    auto t1 = make_tuple(1, 2.0);
    CHECK(std::is_same<decltype(t1), Tuple<int, double>>::value);
  }
}
