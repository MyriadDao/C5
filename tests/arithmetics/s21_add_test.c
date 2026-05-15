#include "../test_cases.h"

void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, 0);
}

void test_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

START_TEST(test_1) {
    // 792281625.14264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    int code = s21_add(decimal1, decimal2, NULL);

    ck_assert_int_ne(code, 0);
}

START_TEST(test_2) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_3) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_4) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{0, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_5) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_6) {
    // степень 28 (что корректно), но биты 0-15 не нули (старший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C8000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_7) {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_8) {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_9) {
    // Просто все единицы
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_10) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal2 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_11) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_12) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{0, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_13) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_14) {
    // степень 28 (что корректно), но биты 0-15 не нули (старший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1C8000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_15) {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_16) {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_17) {
    // Просто все единицы
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_18) {
    // -79228162514264337593543950334
    s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // -79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // overflow


    test_add_fail(decimal1, decimal2, 2);
}

START_TEST(test_19) {
    //  79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFF0, 0xFFFFFFFE, 0xFFFFFFFE, 0x80000000}};
    // 0

    s21_decimal check = {{0x1, 0x0, 0x0, 0x0}};
    s21_decimal result;

   s21_add(decimal1, decimal2, &result);
   printf("%X %X %X E-%d\n", check.bits[2], check.bits[1],
   check.bits[0],
          get(&check, expo));
   printf("%X %X %X E-%d\n", result.bits[2], result.bits[1],
   result.bits[0],
          get(&result, expo));
   printf("%X %X %X E-%d\n", decimal1.bits[2], decimal1.bits[1], decimal1.bits[0],
          get(&decimal1, sign));
   printf("%X %X %X E-%d\n", decimal2.bits[2], decimal2.bits[1], decimal2.bits[0],
          get(&decimal2, sign));

    test_add(decimal1, decimal2, check);
}

START_TEST(test_20) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 7922816251426433759354395033
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_21) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -7922816251426433759354395033
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x80000000}};
    // 71305346262837903834189555302
    s21_decimal check = {{0x66666666, 0x66666666, 0xE6666666, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_22) {
    // -79228162514264337593543950334
    s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // -7922816251426433759354395033
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x80000000}};
    // overflow

    test_add_fail(decimal1, decimal2, 2);
}

TCase* s21_add_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_add");
	
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
	tcase_add_test(tc, test_18);
	tcase_add_test(tc, test_19);
	tcase_add_test(tc, test_20);
	tcase_add_test(tc, test_21);


	return tc;
}
