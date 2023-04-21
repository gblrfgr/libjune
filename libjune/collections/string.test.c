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

static char *test_formatting() {
  lj_string_t fmt = lj_string_from_cstr("The length of this string is %zu!",
                                        &lj_default_allocator);
  lj_string_t formatted = lj_string_format(&fmt, lj_string_length(&fmt));
  lj_assert(lj_string_length(&fmt) == 33, "string length should be 33");
  lj_string_t expected = lj_string_from_cstr("The length of this string is 33!",
                                             &lj_default_allocator);
  lj_assert(lj_string_equals(&formatted, &expected),
            "string formatting should work");
  return 0;
}

static char *test_substrings() {
  lj_string_t original =
      lj_string_from_cstr("Gay rights", &lj_default_allocator);
  lj_string_t gay;
  lj_string_t rights;
  lj_assert(lj_string_substring(&original, 0, 3, &gay), "indices should be valid");
  lj_assert(lj_string_substring(&original, 4, 10, &rights), "indices should be valid");
  lj_string_t expected_gay = lj_string_from_cstr("Gay", &lj_default_allocator);
  lj_string_t expected_rights = lj_string_from_cstr("rights", &lj_default_allocator);
  lj_assert(lj_string_equals(&gay, &expected_gay), "strings should match");
  lj_assert(lj_string_equals(&rights, &expected_rights), "strings should match");
  return 0;
}

int main(const int argc, const char **argv) {
  lj_run_test(test_concatenation);
  lj_run_test(test_formatting);
  lj_run_test(test_substrings);
  lj_finish_tests();
  return 0;
}