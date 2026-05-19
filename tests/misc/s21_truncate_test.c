#include "../test_cases.h"

START_TEST(test_1) {
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

    int code = s21_truncate(decimal, NULL);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}

START_TEST(test_2) {
    s21_decimal decimal = {{0, 0, 0, 1000000000}};
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_3) {
    s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_4) {
    s21_decimal decimal = {{-1, 0, 0, 0x1C0001}};
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_5) {
    s21_decimal decimal = {{-1, 0, 0, 0x11C0000}};
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_6) {
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_7) {
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    s21_decimal result = {0};

    int code = s21_round(decimal, &result);
    ck_assert_int_eq(s21_is_equal(expected, result), 1);
}

START_TEST(test_8) {
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
 s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal result = {0};

    int code = s21_round(decimal, &result);
    ck_assert_int_eq(s21_is_equal(expected, result), 1);
}

START_TEST(test_9) {
  s21_decimal value = {{14, 0, 0, 1u << 16}};

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_10) {
  s21_decimal value = {{15, 0, 0, 1u << 16}};

  s21_decimal expected = {{2, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_11) {
  s21_decimal value = {{15, 0, 0, (1u << 16) | (1u << 31)}};

  s21_decimal expected = {{2, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_12) {
  s21_decimal value = {{100, 0, 0, 0}};

  s21_decimal expected = {{100, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_13) {
  s21_decimal value = {{15, 0, 0, 1u << 16}};

  int code = s21_round(value, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST


TCase* s21_truncate_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_truncate");
	
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


	return tc;
}