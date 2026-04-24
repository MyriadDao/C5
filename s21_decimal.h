#ifndef S21_DECIMAL
#define S21_DECIMAL

#include "s21_decimal_type.h"

int add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

int s21_from_int_to_decimal(int src, s21_decimal* dest);
int s21_from_float_to_decimal(float src, s21_decimal* dest);
int s21_from_decimal_to_int(s21_decimal src, int* dest);
int s21_from_decimal_to_float(s21_decimal src, float* dest);

int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);

#endif
