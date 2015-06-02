#include <echo/htl/utility.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::htl;

TEST_CASE("utility") {
  CHECK(
    std::is_same<
      copy_cv_qualifiers<int&, int>,
      int&
    >::value
  );
}
