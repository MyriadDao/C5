#include "decimal_suite.h"

Suite* decimal_suite()
{
	Suite* suite;

	suite = suite_create("s21_decimal");

	suite_add_tcase(suite, s21_add_test_case());
//	suite_add_tcase();

	return suite;
}
