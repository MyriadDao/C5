#include "../test_cases.h"

START_TEST(test_is_greater_1) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_greater_2) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_greater_3) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_greater_4) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_greater_5) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_greater_6) {
  s21_decimal decimal1 = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_greater_7) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_greater_8) {
  s21_decimal decimal1 = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_greater(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

TCase *s21_is_greater_test_case() {
  TCase *tc;

  tc = tcase_create("s21_is_greater");

  tcase_add_test(tc, test_is_greater_1);
  tcase_add_test(tc, test_is_greater_2);
  tcase_add_test(tc, test_is_greater_3);
  tcase_add_test(tc, test_is_greater_4);
  tcase_add_test(tc, test_is_greater_5);
  tcase_add_test(tc, test_is_greater_6);
  tcase_add_test(tc, test_is_greater_7);
  tcase_add_test(tc, test_is_greater_8);

  return tc;
}