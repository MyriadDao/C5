#include "decimal_suite.h"

Suite* decimal_suite()
{
	Suite* suite;

	suite = suite_create("s21_decimal");

	suite_add_tcase(suite, s21_add_test_case());
	suite_add_tcase(suite, s21_sub_test_case());
	suite_add_tcase(suite, s21_mul_test_case());
	suite_add_tcase(suite, s21_div_test_case());

	suite_add_tcase(suite, s21_is_equal_test_case());
	suite_add_tcase(suite, s21_is_not_equal_test_case());
	suite_add_tcase(suite, s21_is_greater_test_case());
	suite_add_tcase(suite, s21_is_greater_or_equal_test_case());
	suite_add_tcase(suite, s21_is_less_test_case());
	suite_add_tcase(suite, s21_is_less_or_equal_test_case());
//	suite_add_tcase(suite, s21_floor_test_case());

//	suite_add_tcase();

	return suite;
}
