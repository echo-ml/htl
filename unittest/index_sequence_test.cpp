#include <echo/htl/integer_sequence.h>
#include <echo/test.h>
using namespace echo;

TEST_CASE("integer_sequence") {
  using S1 = htl::make_index_sequence<3>;
  using S2 = htl::make_index_sequence<1, 2>;

  type_equal<S1, htl::index_sequence<0, 1, 2>>();
  type_equal<S2, htl::index_sequence<1>>();
}
