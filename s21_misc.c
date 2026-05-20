#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  else if (!s21_is_correct_decimal(value)) return 1;

  int status = s21_truncate(value, result);
  if (status != 0) return 1;

  int sign = !!(value.bits[3] & (1u << 31));

  if (sign) {
    int has_fraction = 0;
    s21_decimal check_val;
    s21_truncate(value, &check_val); 
    
    if (!s21_is_equal(value, check_val)) {
      has_fraction = 1;
    }

    if (has_fraction) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_sub(*result, one, result);
    }
  }

  return 0;
}


int s21_round(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  else if (!s21_is_correct_decimal(value)) return 1;
  if (!s21_truncate(value, result) && !s21_is_equal(value, *result)) {
    if (s21_decimal_get_sign(value))
      s21_sub(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    else
      s21_add(value, ((s21_decimal){{5, 0, 0, 65536}}), &value);
    s21_truncate(value, result);
  }
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;
  else if (!s21_is_correct_decimal(value)) return 1;

  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;

  int scale = (value.bits[3] >> 16) & 0xFF;
  int sign = !!(value.bits[3] & (1u << 31));

  unsigned int mantissa[3];
  mantissa[0] = value.bits[0];
  mantissa[1] = value.bits[1];
  mantissa[2] = value.bits[2];

  for (int y = 0; y < scale; y++) {
    unsigned long long remainder = 0;
    
    for (int x = 2; x >= 0; x--) {
      unsigned long long current = (remainder << 32) + mantissa[x];
      mantissa[x] = (unsigned int)(current / 10);
      remainder = current % 10;
    }
  }

  result->bits[0] = mantissa[0];
  result->bits[1] = mantissa[1];
  result->bits[2] = mantissa[2];

  if (sign) {
    result->bits[3] |= (1u << 31);
  }

  return 0;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  else if (!s21_is_correct_decimal(value)) return 1;

  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }

  int current_sign = !!(value.bits[3] & (1u << 31));

  if (current_sign) {
    result->bits[3] &= ~(1u << 31);
  } else {
    result->bits[3] |= (1u << 31);
  }

  return 0;
}
