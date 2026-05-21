#include "s21_decimal.h"

int s21_get_bit(s21_decimal value, int bit) {
  if (bit < 0 || bit > 127)
    return 0;
  return !!(value.bits[bit / 32] & (1u << (bit % 32)));
}

s21_decimal *s21_set_bit(s21_decimal *value, int pos, int bit) {
  if (value && pos >= 0 && pos <= 127) {
    if (bit)
      value->bits[pos / 32] |= (1u << (pos % 32));
    else
      value->bits[pos / 32] &= ~(1u << (pos % 32));
  }
  return value;
}

int s21_add_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result)
    return 1;
  int exp = 0;

  for (int i = 0; i < 96; i += 1) {
    int bit1 = s21_get_bit(value_1, i);
    int bit2 = s21_get_bit(value_2, i);

    int sum = bit1 + bit2 + exp;

    s21_set_bit(result, i, sum % 2);
    exp = sum / 2;
  }

  return exp;
}

s21_decimal *s21_set_scale(s21_decimal *value, int scale) {
  if (value && scale >= 0 && scale <= 28) {
    int sign = s21_decimal_get_sign(*value);
    value->bits[3] = 0;
    value->bits[3] |= (scale << 16);
    if (sign)
      s21_set_sign(value, 1);
  }
  return value;
}

s21_decimal *s21_increase_scale(s21_decimal *value, int shift) {
  if (!value)
    return value;

  for (int i = 0; i < shift; i++) {
    int current_scale = s21_decimal_get_power(*value);
    if (current_scale >= 28)
      break;

    s21_decimal v_x2 = *value;
    s21_decimal v_x8 = *value;

    s21_left_shift(&v_x2, 1);
    s21_left_shift(&v_x8, 3);

    s21_decimal res_mantissa = {{0, 0, 0, 0}};
    int overflow = s21_add_bit(v_x2, v_x8, &res_mantissa);

    if (overflow) {
      break;
    }

    for (int x = 0; x < 3; x++)
      value->bits[x] = res_mantissa.bits[x];
    s21_set_scale(value, current_scale + 1);
  }
  return value;
}

s21_decimal *s21_decrease_scale(s21_decimal *value, int shift) {
  if (!value)
    return value;

  for (int y = 0; y < shift; y += 1) {
    int current_scale = s21_decimal_get_power(*value);
    if (current_scale == 0)
      break;

    unsigned long long remainder = 0;
    for (int x = 2; x >= 0; x -= 1) {
      unsigned long long current = (remainder << 32) + value->bits[x];
      value->bits[x] = current / 10;
      remainder = current % 10;
    }
    s21_set_scale(value, current_scale - 1);
  }
  return value;
}

void s21_alignment_scale(s21_decimal *value_1, s21_decimal *value_2) {
  if (!value_1 || !value_2)
    return;

  int scale1 = s21_decimal_get_power(*value_1);
  int scale2 = s21_decimal_get_power(*value_2);

  if (scale1 == scale2)
    return;

  while (scale1 < scale2) {
    s21_decimal check = *value_1;
    s21_increase_scale(&check, 1);

    if (s21_decimal_get_power(check) > scale1) {
      *value_1 = check;
      scale1 = s21_decimal_get_power(*value_1);
    } else {
      s21_decrease_scale(value_2, 1);
      scale2 = s21_decimal_get_power(*value_2);
    }
  }

  while (scale2 < scale1) {
    s21_decimal check = *value_2;
    s21_increase_scale(&check, 1);

    if (s21_decimal_get_power(check) > scale2) {
      *value_2 = check;
      scale2 = s21_decimal_get_power(*value_2);
    } else {
      s21_decrease_scale(value_1, 1);
      scale1 = s21_decimal_get_power(*value_1);
    }
  }
}

s21_decimal *s21_set_sign(s21_decimal *value, int bit) {
  if (value) {
    if (bit)
      value->bits[3] |= (1u << 31);
    else
      value->bits[3] &= ~(1u << 31);
  }
  return value;
}

int s21_is_null(s21_decimal value) {
  return !value.bits[0] && !value.bits[1] && !value.bits[2];
}

s21_decimal *s21_left_shift(s21_decimal *value, int shift) {
  if (!value)
    return value;

  for (int y = 0; y < shift; y += 1) {
    int transfer_0_to_1 = !!(value->bits[0] & (1u << 31));
    int transfer_1_to_2 = !!(value->bits[1] & (1u << 31));

    value->bits[0] <<= 1;
    value->bits[1] <<= 1;
    value->bits[2] <<= 1;

    if (transfer_0_to_1)
      value->bits[1] |= 1u;
    if (transfer_1_to_2)
      value->bits[2] |= 1u;
  }
  return value;
}

int s21_is_correct_decimal(s21_decimal decimal) {
  int code = 1;

  if (s21_decimal_get_empty1(decimal) != 0 ||
      s21_decimal_get_empty2(decimal) != 0) {
    code = 0;
  } else {
    int power = s21_decimal_get_power(decimal);
    if (power < 0 || power > 28) {
      code = 0;
    }
  }

  return code;
}

int s21_decimal_get_empty1(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.empty1;
}

int s21_decimal_get_empty2(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.empty2;
}

int s21_decimal_get_power(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.power;
}

int s21_decimal_get_sign(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.sign;
}
