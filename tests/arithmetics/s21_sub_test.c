#include "../test_cases.h"

START_TEST(test_1) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    int code = s21_sub(decimal1, decimal2, NULL);

    ck_assert_int_ne(code, 0);
}

START_TEST(test_2) {
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_3) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_4) {
  s21_decimal val1 = {{4, 0, 0, 0}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_sub(val1, val2, &res));
}
END_TEST

START_TEST(test_5) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_6) {
  s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_sub(val1, val2, &res));
}
END_TEST

START_TEST(test_7) {
    s21_decimal decimal1 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_8) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(1, s21_sub(val1, val2, &res));
}
END_TEST

START_TEST(test_9) {
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, 4);
}
END_TEST

START_TEST(test_10) {
  s21_decimal val1 = {{4, 0, 0, 0}};
  s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(1, s21_sub(val1, val2, &res));
}
END_TEST

START_TEST(test_11) {
  s21_decimal dec1;
  s21_decimal dec2;

  int tmp1 = 100;
  int tmp2 = 99999;

  int res_s21 = 0;
  int res = -99899;

  s21_decimal res1;
  s21_from_int_to_decimal(tmp1, &dec1);
  s21_from_int_to_decimal(tmp2, &dec2);

  s21_sub(dec1, dec2, &res1);
  s21_from_decimal_to_int(res1, &res_s21);

  ck_assert_int_eq(res_s21, res);
}
END_TEST

START_TEST(test_12) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{4, 0, 0, 0}};
  s21_decimal res;

  ck_assert_int_eq(2, s21_sub(val1, val2, &res));
}
END_TEST

START_TEST(test_13) {
  s21_decimal val1 = {{8, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;

  ck_assert_int_eq(0, s21_sub(val1, val2, &res));
}
END_TEST

TCase* s21_sub_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_sub");
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