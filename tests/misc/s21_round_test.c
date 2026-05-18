#include "../test_cases.h"

START_TEST(test_1) {
  s21_decimal value = {{14, 0, 0, 1u << 16}};

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_2) {
    // 792281625.14264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

    int code = s21_round(decimal, NULL);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}

START_TEST(test_3) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal = {{0, 0, 0, 1000000000}};
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_4) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_5) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal = {{-1, 0, 0, 0x1C0001}};
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_6) {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal = {{-1, 0, 0, 0x11C0000}};
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_7) {
    // Просто все единицы
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_8) {
  s21_decimal value = {{15, 0, 0, 1u << 16}};

  s21_decimal expected = {{2, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_9) {
  s21_decimal value = {{15, 0, 0, (1u << 16) | (1u << 31)}};

  s21_decimal expected = {{2, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_10) {
  s21_decimal value = {{100, 0, 0, 0}};

  s21_decimal expected = {{100, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_round(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

TCase* s21_round_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_round");
	
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
/*
	tcase_add_test(tc, test_11);
	tcase_add_test(tc, test_12);
	tcase_add_test(tc, test_13);
	tcase_add_test(tc, test_14);
	tcase_add_test(tc, test_15);
*/

	return tc;
}