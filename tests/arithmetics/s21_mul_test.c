#include "../test_cases.h"

START_TEST(test_1) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    int code = s21_mul(decimal1, decimal2, NULL);

    ck_assert_int_ne(code, 0);
}

START_TEST(test_2) {
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_3) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_4) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {{0}};

  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_5) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_6) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};

  ck_assert_int_eq(2, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_7) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_8) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_9) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_10) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};

  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_11) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0}};

  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_12) {
  s21_decimal val1 = {{8, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_13) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val2 = {{200, 0, 0, 0}};
  s21_decimal res = {{0}};

  s21_set_scale(&val2, 1);
  s21_set_scale(&val2, 2);

  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(test_14) {
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val1 = {{20, 0, 0, 0}};
  s21_decimal res = {{0}};

  s21_set_scale(&val2, 1);
  s21_set_scale(&val2, 2);

  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

TCase* s21_mul_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_mul");
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
