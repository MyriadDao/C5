#include "../test_cases.h"

START_TEST(test_1) {
  float src = 0.0f;

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_2) {
  float src = -0.0f;

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_3) {
  float src = 123.0f;

  s21_decimal expected = {{123, 0, 0, 0}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_4) {
  float src = -123.0f;

  s21_decimal expected = {{123, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_5) {
  float src = 12.5f;

  s21_decimal expected = {{125, 0, 0, 1u << 16}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_6) {
  float src = -12.5f;

  s21_decimal expected = {{125, 0, 0, (1u << 31) | (1u << 16)}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_7) {
  float src = 0.0125f;

  s21_decimal expected = {{125, 0, 0, 4u << 16}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_8) {
  float src = 1e-29f;

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {{1, 1, 1, 1}};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_9) {
  float src = 1e15f;

  s21_decimal result = {0};

  int code = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_10) {
  float src = INFINITY;

  s21_decimal result = {0};

  int code = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_11) {
  float src = -INFINITY;

  s21_decimal result = {0};

  int code = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_12) {
  float src = NAN;

  s21_decimal result = {0};

  int code = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_13) {
  float src = 1.2345665f;

  s21_decimal expected = {{1234566, 0, 0, 6u << 16}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_14) {
  float src = 123.45f;

  int code = s21_from_float_to_decimal(src, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_15) {
  float src = 1.234567f;

  s21_decimal expected = {{1234567, 0, 0, 6u << 16}};
  s21_decimal result = {0};

  s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

TCase *s21_from_float_to_decimal_test_case() {
  TCase *tc;

  tc = tcase_create("s21_from_float_to_decimal");
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
  tcase_add_test(tc, test_12);
  tcase_add_test(tc, test_13);
  tcase_add_test(tc, test_14);
  tcase_add_test(tc, test_15);

  return tc;
}