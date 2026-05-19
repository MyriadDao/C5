#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater(value_2, value_1);
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int sign_a = getSign(value_1), sign_b = getSign(value_2);
  if (!(isNull(value_1) && isNull(value_2))) {
    if (sign_a != sign_b) {
      if (sign_a == 0) {
        res = 1;
      } else {
        res = 0;
      }
    } else {
      alignmentScale(&value_1, &value_2);
      for (int i = 95; i >= 0; i--) {
        int bit_a = getBit(value_1, i), bit_b = getBit(value_2, i);
        if (bit_a != bit_b) {
          if (bit_a != 0) {
            res = (sign_a == 0) ? 1 : 0;
            break;
          } else {
            res = (sign_a == 0) ? 0 : 1;
            break;
          }
        }
      }
    }
  }
  return res;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2] &&
      value_1.bits[3] == value_2.bits[3]) {
    return 1; 
  }

  if (isNull(value_1) && isNull(value_2)) {
    return 1;
  }
  
  if (getSign(value_1) != getSign(value_2)) {
    return 0;
  }
  
  if (getScale(value_1) != getScale(value_2)) {
    alignmentScale(&value_1, &value_2);
  }
  
  return (value_1.bits[0] == value_2.bits[0] &&
          value_1.bits[1] == value_2.bits[1] &&
          value_1.bits[2] == value_2.bits[2] &&
          getScale(value_1) == getScale(value_2));
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
