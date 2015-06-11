#include <echo/htl/functional.h>
#include <echo/test.h>
using namespace echo;
using namespace echo::htl;
using namespace echo::htl::functional;

TEST_CASE("functional") {
  CHECK(and_(true, true));
}
