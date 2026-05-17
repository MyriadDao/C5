#include "../test_cases.h"

void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result = {0};
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
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 1);
}

START_TEST(test_2) {
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_3) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_4) {
  s21_decimal val1 = {{15, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_5) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(2, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_6) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C8000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;

    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_7) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{8, 0, 0, 0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_8) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_9) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_10) {
  s21_decimal val1 = {{0}};
  s21_decimal val2 = {{0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_11) {
  s21_decimal val1 = {{4, 0, 0, 0}};
  s21_decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_12) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(2, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_13) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_14) {
  s21_decimal val1 = {{4, 0, 0, 0}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_15) {
  s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_16) {
  s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(2, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_17) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};

  setScale(&val1, 5);
  setScale(&val2, 3);

  s21_decimal res;
  ck_assert_int_eq(0, s21_add(val1, val2, &res));
}
END_TEST

START_TEST(test_18) {
    s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};


    test_add_fail(decimal1, decimal2, 2);
}

START_TEST(test_19) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

    s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_20) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};

    test_add_fail(decimal1, decimal2, 1);
}

START_TEST(test_21) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    s21_decimal check = {{0x1, 0x0, 0x0, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_22) {
    s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
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
	tcase_add_test(tc, test_22);
	tcase_add_test(tc, test_21);


	return tc;
}
