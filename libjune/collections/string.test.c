#include <libjune/collections/string.h>
#include <libjune/unit.h>
#include <stdio.h>

static char *test_concatenation() {
  lj_string_t a = lj_string_from_cstr("Hello, ", &lj_default_allocator);
  lj_string_t b = lj_string_from_cstr("world!", &lj_default_allocator);
  lj_assert(!lj_string_equals(&a, &b),
            "different strings should not compare equal");
  lj_string_t expected =
      lj_string_from_cstr("Hello, world!", &lj_default_allocator);
  lj_string_t actual;
  lj_assert(lj_string_concatenate(&a, &b, &actual),
            "string concatenation should be successful");
  lj_assert(lj_string_equals(&actual, &expected), "concatenation should work");
  return 0;
}

int main(const int argc, const char **argv) {
  lj_run_test(test_concatenation);
  lj_finish_tests();
  return 0;
}