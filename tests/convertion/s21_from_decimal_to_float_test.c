#include "../test_cases.h"

START_TEST(test_1) {
  s21_decimal value = {{123, 0, 0, 0}};

  float expected = 123.0f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_2) {
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  int code = s21_from_decimal_to_float(decimal, NULL);

  ck_assert_int_eq(code, 1);
}

START_TEST(test_3) {
  s21_decimal value = {{123, 0, 0, 2 << 16}};

  float expected = 1.23f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_4) {
  s21_decimal value = {{123, 0, 0, (2u << 16) | (1u << 31)}};

  float expected = -1.23f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_5) {
  s21_decimal value = {{0, 0, 0, 0}};

  float expected = 0.0f;
  float result = -1;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_6) {
  s21_decimal value = {{0, 0, 0, 1u << 31}};

  float expected = 0.0f;
  float result = 1;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_7) {
  s21_decimal value = {{123456789, 0, 0, 0}};

  float expected = 123456789.0f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_8) {
  s21_decimal value = {{1, 0, 0, 6 << 16}};

  float expected = 0.000001f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_9) {
  s21_decimal value = {{1, 0, 0, 28 << 16}};

  float expected = 1e-28f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_10) {
  s21_decimal src = {{123, 0, 0, 0}};

  int code = s21_from_decimal_to_float(src, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_11) {
  s21_decimal value = {{1, 0, 0, (6u << 16) | (1u << 31)}};

  float expected = -0.000001f;
  float result = 0;

  s21_from_decimal_to_float(value, &result);

  ck_assert_float_eq(expected, result);
}
END_TEST

START_TEST(test_12) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

  float result = 0;

  int code = s21_from_decimal_to_float(value, &result);

  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_13) {
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  int code = s21_from_decimal_to_float(decimal, NULL);

  ck_assert_int_eq(code, 1);
}

START_TEST(test_14) {
  s21_decimal decimal = {{0, 0, 0, 1000000000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_15) {
  s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_16) {
  s21_decimal decimal = {{-1, 0, 0, 0x1C8000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_17) {
  s21_decimal decimal = {{-1, 0, 0, 0x11C0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

TCase *s21_from_decimal_to_float_test_case() {
  TCase *tc;

  tc = tcase_create("s21_from_decimal_to_float");
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
  tcase_add_test(tc, test_16);
  tcase_add_test(tc, test_17);

  return tc;
}