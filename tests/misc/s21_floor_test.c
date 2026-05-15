#include "../test_cases.h"

void test_floor(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_floor(decimal, &result);
    //int sign_check = test_decimal_get_sign(decimal_check);
    int sign_check = get(&decimal_check, sign);
    //int sign_result = test_decimal_get_sign(result);
    int sign_result = get(&result, sign);

    ck_assert_int_eq(code, TEST_OTHER_OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(sign_check, sign_result);
}

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
    s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_floor(decimal, decimal_check);
}

START_TEST(test_floor_6) {
    // -79228162514264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // -79228162514264337593543950335
    s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

    test_floor(decimal, decimal_check);
}

START_TEST(test_floor_7) {
    // 7922816251426433759354395033.5
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
    // 7922816251426433759354395033
    s21_decimal decimal_check = {{0x99999999, 0x99999999, 0x19999999, 0x0}};

    test_floor(decimal, decimal_check);
}

START_TEST(test_floor_8) {
    // -7922816251426433759354395033.5
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
    // -7922816251426433759354395034
    s21_decimal decimal_check = {{0x9999999A, 0x99999999, 0x19999999, 0x80000000}};

    test_floor(decimal, decimal_check);
}

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

	return tc;
}