#ifndef UTILS
#define UTILS

#include "s21_decimal.h"

int s21_get_bit(s21_decimal value, int bit);
s21_decimal *s21_set_bit(s21_decimal *value, int bit, int a);
int s21_add_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

s21_decimal *s21_set_scale(s21_decimal *d, int scale);
s21_decimal *s21_increase_scale(s21_decimal *value, int shift);
s21_decimal *s21_decrease_scale(s21_decimal *value, int shift);
void s21_alignment_scale(s21_decimal *value_1, s21_decimal *value_2);

s21_decimal *s21_set_sign(s21_decimal *value, int bit);

int s21_is_null(s21_decimal value);
s21_decimal *s21_left_shift(s21_decimal *value, int shift);

int s21_decimal_get_power(s21_decimal decimal);
int s21_decimal_get_sign(s21_decimal decimal);
int s21_decimal_get_empty1(s21_decimal decimal);
int s21_decimal_get_empty2(s21_decimal decimal);
int s21_is_correct_decimal(s21_decimal decimal);

#endif
