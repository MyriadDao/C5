#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal_type.h"
#include "s21_utils.h"

#define ONE_DEC ((s21_decimal){{1, 0, 0, 0}})
#define ZERO_DEC ((s21_decimal){{0, 0, 0, 0}})

#define SUCCESS 0
#define CALCULATION_ERROR 1

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

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
