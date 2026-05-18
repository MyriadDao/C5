#include "../test_cases.h"

START_TEST(test_1) {
  s21_decimal src = {{0, 0, 0, 0}};

  int expected = 0;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_2) {
  s21_decimal src = {{123, 0, 0, 0}};

  int expected = 123;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_3) {
  s21_decimal src = {{123, 0, 0, 1u << 31}};

  int expected = -123;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_4) {
  s21_decimal src = {{99, 0, 0, 1u << 16}};

  int expected = 9;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_5) {
  s21_decimal src = {{99, 0, 0, (1u << 31) | (1u << 16)}};

  int expected = -9;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_6) {
  s21_decimal src = {{1239, 0, 0, 1u << 16}};

  int expected = 123;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_7) {
  s21_decimal src = {{2147483647, 0, 0, 0}};

  int expected = 2147483647;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_8) {
  s21_decimal src = {{2147483648u, 0, 0, 1u << 31}};

  int expected = (-2147483647 - 1);
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_9) {
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};

  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_10) {
  s21_decimal src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 31}};

  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_11) {
  s21_decimal src = {{1, 0, 0, 28 << 16}};

  int expected = 0;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_12) {
  s21_decimal src = {{1000, 0, 0, 3u << 16}};

  int expected = 1;
  int result = 0;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_13) {
  s21_decimal src = {{0, 0, 0, 1u << 31}};

  int expected = 0;
  int result = 1;

  int code = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_14) {
  s21_decimal src = {{123, 0, 0, 0}};

  int code = s21_from_decimal_to_int(src, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST


TCase* s21_from_decimal_to_int_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_from_decimal_to_int");
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

	return tc;
}