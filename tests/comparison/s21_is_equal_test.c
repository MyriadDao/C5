#include "../test_cases.h"

START_TEST(test_is_equal_1) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_equal_2) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFF0F, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFF0F, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_equal_3) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_equal_4) {
  s21_decimal decimal1 = {{0x0, 0x0, 0x0, 0x0}};
  ;
  s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x0}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_TRUE);
}

START_TEST(test_is_equal_5) {
  s21_decimal decimal1 = {{0x64B0BF42, 0x2A3886D9, 0x62D0E25A, 0x110000}};
  s21_decimal decimal2 = {{0xC8340001, 0x8881186E, 0x4A03CE6, 0x120000}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_equal_6) {
  s21_decimal decimal1 = {{0xC8340001, 0x8881186E, 0x4A03CE6, 0x120000}};
  s21_decimal decimal2 = {{0x64B0BF42, 0x2A3886D9, 0x62D0E25A, 0x110000}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_equal_7) {
  s21_decimal decimal1 = {{0x64B0BF42, 0x2A3886D9, 0x62D0E25A, 0x110000}};
  s21_decimal decimal2 = {{0xC8340001, 0x8881186E, 0x4A03CE6, 0x80120000}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

START_TEST(test_is_equal_8) {
  s21_decimal decimal1 = {{0xC8340001, 0x8881186E, 0x4A03CE6, 0x80120000}};
  s21_decimal decimal2 = {{0x64B0BF42, 0x2A3886D9, 0x62D0E25A, 0x110000}};

  ck_assert_int_eq(s21_is_equal(decimal1, decimal2), TEST_COMPARISON_FALSE);
}

TCase *s21_is_equal_test_case() {
  TCase *tc;

  tc = tcase_create("s21_is_equal");

  tcase_add_test(tc, test_is_equal_1);
  tcase_add_test(tc, test_is_equal_2);
  tcase_add_test(tc, test_is_equal_3);
  tcase_add_test(tc, test_is_equal_4);
  tcase_add_test(tc, test_is_equal_5);
  tcase_add_test(tc, test_is_equal_6);
  tcase_add_test(tc, test_is_equal_7);
  tcase_add_test(tc, test_is_equal_8);

  return tc;
}