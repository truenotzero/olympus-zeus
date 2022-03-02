#include "src/cpu.c"
#include <test.inc>

TEST_UNUSED_INIT()
TEST_UNUSED_CLEANUP()

struct test *register_tests() {
  static struct test tests[] = {
      TEST_DONE(),
  };

  return tests;
}
