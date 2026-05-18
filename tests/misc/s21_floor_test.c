#include "../test_cases.h"


START_TEST(test_floor_1) {
    // 792281625.14264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

    int code = s21_floor(decimal, NULL);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}

START_TEST(test_floor_2) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal = {{0, 0, 0, 1000000000}};
    s21_decimal result;

    int code = s21_floor(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_floor_3) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};
    s21_decimal result;

    int code = s21_floor(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_floor_4) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal = {{0, 0, 0, 0x1D0000}};
    s21_decimal result;

    int code = s21_floor(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_floor_5) {
    // 79228162514264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 79228162514264337593543950335
    s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    s21_decimal result;
    s21_floor(decimal, &result);

    ck_assert_int_eq(s21_is_equal(check, result), 1);
}

START_TEST(test_floor_6) {
    // -79228162514264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // -79228162514264337593543950335
    s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

    s21_decimal result;
    s21_floor(decimal, &result);

    ck_assert_int_eq(s21_is_equal(check, result), 1);
}

START_TEST(test_floor_7) {
    // 7922816251426433759354395033.5
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
    // 7922816251426433759354395033
    s21_decimal check = {{0x99999999, 0x99999999, 0x19999999, 0x0}};

    s21_decimal result;
    s21_floor(decimal, &result);

    ck_assert_int_eq(s21_is_equal(check, result), 1);
}

START_TEST(test_floor_8) {
    // -7922816251426433759354395033.5
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
    // -7922816251426433759354395034
    s21_decimal check = {{0x9999999A, 0x99999999, 0x19999999, 0x80000000}};

    s21_decimal result;
    s21_floor(decimal, &result);

    ck_assert_int_eq(s21_is_equal(check, result), 1);
}

START_TEST(test_floor_9) {
  s21_decimal value = {{19, 0, 0, 1u << 16}};

  s21_decimal expected = {{1, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_floor_10) {
  s21_decimal value = {{19, 0, 0, (1u << 16) | (1u << 31)}};

  s21_decimal expected = {{2, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_floor_11) {
  s21_decimal value = {{15, 0, 0, 0}};

  s21_decimal expected = {{15, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_floor_12) {
  s21_decimal value = {{0, 0, 0, 0}};

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_floor_13) {
  s21_decimal value = {{15, 0, 0, 0}};

  int code = s21_floor(value, NULL);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_floor_14) {
  s21_decimal value = {{15, 0, 0, 1u << 31}};

  s21_decimal expected = {{15, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

START_TEST(test_floor_15) {
  s21_decimal value = {{1, 0, 0, (1u << 16) | (1u << 31)}};

  s21_decimal expected = {{1, 0, 0, 1u << 31}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(s21_is_equal(expected, result), 1);
}
END_TEST

TCase* s21_floor_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_floor");
	
	tcase_add_test(tc, test_floor_1);
	tcase_add_test(tc, test_floor_2);
	tcase_add_test(tc, test_floor_3);
	tcase_add_test(tc, test_floor_4);
	tcase_add_test(tc, test_floor_5);
	tcase_add_test(tc, test_floor_6);
	tcase_add_test(tc, test_floor_7);
	tcase_add_test(tc, test_floor_8);
	tcase_add_test(tc, test_floor_9);
	tcase_add_test(tc, test_floor_10);
	tcase_add_test(tc, test_floor_11);
	tcase_add_test(tc, test_floor_12);
	tcase_add_test(tc, test_floor_13);
	tcase_add_test(tc, test_floor_14);
	tcase_add_test(tc, test_floor_15);


	return tc;
}
