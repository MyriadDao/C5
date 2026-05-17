#include "../test_cases.h"

START_TEST(test_1) {
  int src = 0;

  s21_decimal expected = {{0, 0, 0, 0}};
  s21_decimal result = {0};

  int code = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(s21_is_equal(result, expected), 0);
}
END_TEST


TCase* s21_from_int_to_decimal_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_from_int_to_decimal");
	tcase_add_test(tc, test_1);
/*
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
*/

	return tc;
}