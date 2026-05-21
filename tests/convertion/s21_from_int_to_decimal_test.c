#include "../test_cases.h"

START_TEST(test_1) {
  int src = 0;

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_2) {
  int src = 123;

  s21_decimal expected = {{123, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_3) {
  int src = -123;

  s21_decimal expected = {{123, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_4) {
  int src = 1;

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_5) {
  int src = -1;

  s21_decimal expected = {{1, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_6) {
  int src = 2147483647;

  s21_decimal expected = {{2147483647, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_7) {
  int src = (-2147483647 - 1);

  s21_decimal expected = {{2147483648u, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_8) {
  int src = 999;

  s21_decimal expected = {{999, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_9) {
  int src = 777;

  s21_decimal expected = {{777, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_10) {
  int src = -777;

  s21_decimal expected = {{777, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_11) {
  int src = 123;

  int code = s21_from_int_to_decimal(src, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST

TCase *s21_from_int_to_decimal_test_case() {
  TCase *tc;

  tc = tcase_create("s21_from_int_to_decimal");
  tcase_add_test(tc, test_1);
  tcase_add_test(tc, test_2);
  tcase_add_test(tc, test_3);

  tcase_add_test(tc, test_4);
  tcase_add_test(tc, test_5);
  tcase_add_test(tc, test_6);
  tcase_add_test(tc, test_7);
  tcase_add_test(tc, test_8);
  tcase_add_test(tc, test_9);

  tcase_add_test(tc, test_10);
  tcase_add_test(tc, test_11);

  return tc;
}