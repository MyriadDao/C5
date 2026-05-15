#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (!s21_truncate(value, s21_reset(result)) && getSign(value) &&
      !s21_is_equal(*result, value))
    s21_sub(*result, ((s21_decimal){{1, 0, 0, 0}}), result);
  return 0;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  if (!s21_truncate(value, result) && !s21_is_equal(value, *result)) {
    if (getSign(value))
      s21_sub(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    else
      s21_add(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    s21_truncate(value, result);
  }
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_copy(s21_reset(result),
           getScale(value) ? *decreaseScale(&value, getScale(value)) : value);
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  s21_copy(s21_reset(result),
           getSign(value) ? *setSign(&value, 0) : *setSign(&value, 1));
  return 0;
}

s21_decimal* s21_copy(s21_decimal* dest, s21_decimal src) {
  for (int x = 0; x < 4; x += 1) dest->bits[x] = src.bits[x];
  return dest;
}

s21_decimal* s21_reset(s21_decimal* value) {
  for (int x = 0; x < 4; x += 1) value->bits[x] = 0;
  return value;
}
