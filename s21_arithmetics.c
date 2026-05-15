#include "s21_decimal.h"

int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
             s21_decimal *tmp);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = 0, valueSign_1 = getSign(value_1),
      valueSign_2 = getSign(value_2);
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  if (valueSign_1 == valueSign_2) {
    alignmentScale(&value_1, &value_2);
    if (addBit(value_1, value_2, result)) {
      if ((!getScale(value_1) || !getScale(value_2)) && !valueSign_1) {
        status = 1;
      } else if ((!getScale(value_1) || !getScale(value_2)) && valueSign_1) {
        status = 2;
      } else if (getScale(value_1) > 0 && getScale(value_2) > 0) {
        setScale(decreaseScale(&value_1, 1), getScale(value_1) - 1);
        setScale(decreaseScale(&value_2, 1), getScale(value_2) - 1);
        status = s21_add(value_1, value_2, result);
      }
    } else {
      setScale(setSign(result, valueSign_1), getScale(value_1));
    }
  } else if (valueSign_1 && !valueSign_2) {
    status = s21_sub(value_2, *setSign(&value_1, 0), result);
  } else if (!valueSign_1 && valueSign_2) {
    status = s21_sub(value_1, *setSign(&value_2, 0), result);
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = 0, valueSign_1 = getSign(value_1),
      valueSign_2 = getSign(value_2);
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;

  if (valueSign_1 != valueSign_2) {
    status = s21_add(*setSign(&value_1, 0), *setSign(&value_2, 0), result);
    status = status ? (valueSign_1 ? 2 : 1) : status;
    setSign(result, valueSign_1);
  } else {
    alignmentScale(&value_1, &value_2);
    if (!s21_is_equal(value_1, value_2)) {
      setSign(&value_1, 0);
      setSign(&value_2, 0);
      if (s21_is_less(value_1, value_2)) {
        status = s21_add(*convert(&value_1), value_2, result);
        setSign(result, !valueSign_2);
      } else {
        status = s21_add(*convert(&value_2), value_1, result);
        setSign(result, valueSign_1);
      }
    }
  }
  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 0;
  int sign_result = 5;
  if (getSign(value_1) != getSign(value_2)) {
    sign_result = 1;
  } else {
    sign_result = 0;
  }
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  int bit1 = getBitLast(value_1);

  for (int i = 0; i <= bit1; i++) {
    s21_decimal tmp_res = {{0, 0, 0, 0}};
    int value_bit_1 = getBit(value_1, i);

    if (value_bit_1) {
      tmp_res = value_2;
      leftShift(&tmp_res, i);
      res = addBit(*result, tmp_res, result);
    }
  }
  if (res) {
    while (getScale(value_1) > 0 || getScale(value_2) > 0) {
      s21_decimal *value_1_1, *value_2_1;
      if (getBitLast(value_1) > getBitLast(value_2) && getScale(value_1) > 0) {
        value_1_1 = &value_1;
        value_2_1 = &value_2;
      } else if (getBitLast(value_2) > getBitLast(value_1) &&
                 getScale(value_2) > 0) {
        value_1_1 = &value_2;
        value_2_1 = &value_1;
      } else {
        break;
      }
      int scale2 = getScale(*value_1_1);
      decreaseScale(value_1_1, 1);
      setScale(value_1_1, --scale2);
      return s21_mul(*value_1_1, *value_2_1, result);
    }
  }

  int scale = getScale(value_1) + getScale(value_2);
  setScale(result, scale);
  setSign(result, sign_result);
  if (res && sign_result) res = 2;
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  int res = 1;
  int sign = 0;
  int sign1 = getSign(value_1);
  int sign2 = getSign(value_2);
  if (sign1 != sign2) sign = 1;
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  if (!value_2.bits[0] && !value_2.bits[1] && !value_2.bits[2]) {
    res = 3;
  } else {
    s21_decimal tmp = {0};
    setScale(&value_1, 0);
    setScale(&value_2, 0);
    s21_div1(value_1, value_2, result, &tmp);
    res = 0;
  }
  setSign(result, sign);
  return res;
}

int s21_div1(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
             s21_decimal *tmp) {
  int res = 0;
  for (int i = getBitLast(value_1); i >= 0; i--) {
    if (getBit(value_1, i)) setBit(tmp, 0, 1);
    if (s21_is_greater_or_equal(*tmp, value_2)) {
      s21_sub(*tmp, value_2, tmp);
      if (i != 0) leftShift(tmp, 1);
      if (getBit(value_1, i - 1)) setBit(tmp, 0, 1);
      leftShift(result, 1);
      setBit(result, 0, 1);
    } else {
      leftShift(result, 1);
      if (i != 0) leftShift(tmp, 1);
      if ((i - 1) >= 0 && getBit(value_1, i - 1)) setBit(tmp, 0, 1);
    }
  }
  return res;
}