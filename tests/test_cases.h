#ifndef DECIMAL_TEST_CASES
#define DECIMAL_TEST_CASES

#include <check.h>

#include "../s21_decimal.h"

#define TEST_COMPARISON_FALSE 0
#define TEST_COMPARISON_TRUE  1

#define TEST_OTHER_OK 0
#define TEST_OTHER_ERROR 1

TCase* s21_add_test_case();
TCase* s21_sub_test_case();
TCase* s21_mul_test_case();
TCase* s21_div_test_case();

TCase* s21_is_equal_test_case();
TCase* s21_is_not_equal_test_case();
TCase* s21_is_greater_test_case();
TCase* s21_is_greater_or_equal_test_case();
TCase* s21_is_less_test_case();
TCase* s21_is_less_or_equal_test_case();

TCase* s21_from_int_to_decimal_test_case();
TCase* s21_from_float_to_decimal_test_case();
TCase* s21_from_decimal_to_int_test_case();
TCase* s21_from_decimal_to_float_test_case();

TCase* s21_floor_test_case();
TCase* s21_round_test_case();
TCase* s21_truncate_test_case();
TCase* s21_negate_test_case();

#endif
