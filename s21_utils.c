#include "s21_decimal.h"

int getBit(s21_decimal value, int bit) {
  if (bit < 0 || bit > 127) return 0;
  return !!(value.bits[bit / 32] & (1u << (bit % 32)));
}

s21_decimal *setBit(s21_decimal *value, int pos, int bit) {
  if (value && pos >= 0 && pos <= 127) {
    if (bit)
      value->bits[pos / 32] |= (1u << (pos % 32));
    else
      value->bits[pos / 32] &= ~(1u << (pos % 32));
  }
  return value;
}

int getBitLast(s21_decimal value) {
  int bitLast = 95;
  for (; (bitLast >= 0) && (!getBit(value, bitLast));) {
    bitLast -= 1;
  }
  return bitLast;
}


//  TODO [addBit] Необходим рефакторинг кода.
int addBit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 1;
  int exp = 0;
  
  for (int i = 0; i < 96; i += 1) {
    int bit1 = getBit(value_1, i);
    int bit2 = getBit(value_2, i);
    
    int sum = bit1 + bit2 + exp;
    
    setBit(result, i, sum % 2); // Записываем текущий бит (0 или 1)
    exp = sum / 2;             // Перенос в следующий разряд (0 или 1)
  }
  
  return exp; // Возвращает 1, если произошло переполнение мантиссы (96-й бит)
}

int getScale(s21_decimal value) {
  return (value.bits[3] >> 16) & 0xFF;
}

s21_decimal *setScale(s21_decimal *value, int scale) {
  if (value && scale >= 0 && scale <= 28) {
    int sign = getSign(*value);
    value->bits[3] = 0; // Полностью очищаем служебный инт
    value->bits[3] |= (scale << 16);
    if (sign) setSign(value, 1);
  }
  return value;
}

s21_decimal *increaseScale(s21_decimal *value, int shift) {
  if (!value) return value;
  
  for (int i = 0; i < shift; i++) {
    int current_scale = getScale(*value);
    if (current_scale >= 28) break;

    s21_decimal v_x2 = *value;
    s21_decimal v_x8 = *value;
    
    // x10 = (x << 1) + (x << 3)
    leftShift(&v_x2, 1);
    leftShift(&v_x8, 3);
    
    s21_decimal res_mantissa = {{0, 0, 0, 0}};
    int overflow = addBit(v_x2, v_x8, &res_mantissa);
    
    if (overflow) {
      break; // Если мантисса переполнилась, увеличивать scale больше нельзя
    }
    
    // Копируем мантиссу и обновляем scale
    for (int x = 0; x < 3; x++) value->bits[x] = res_mantissa.bits[x];
    setScale(value, current_scale + 1);
  }
  return value;
}

s21_decimal *decreaseScale(s21_decimal *value, int shift) {
  if (!value) return value;
  
  for (int y = 0; y < shift; y += 1) {
    int current_scale = getScale(*value);
    if (current_scale == 0) break;
    
    unsigned long long remainder = 0;
    // Идем от старшего инта к младшему
    for (int x = 2; x >= 0; x -= 1) {
      unsigned long long current = (remainder << 32) + value->bits[x];
      value->bits[x] = current / 10;
      remainder = current % 10;
    }
    setScale(value, current_scale - 1);
  }
  return value;
}


void alignmentScale(s21_decimal *value_1, s21_decimal *value_2) {
  if (getScale(*value_1) != getScale(*value_2)) {
    if (getScale(*value_1) < getScale(*value_2)) {
      alignmentScale(value_2, value_1);
    } else {
      int scaleLow = getScale(*value_2), scaleHigh = getScale(*value_1);
      for (; (scaleHigh - scaleLow) && !getBit(*value_2, 96); scaleLow += 1)
        increaseScale(value_2, 1);
      for (; scaleHigh - scaleLow; scaleHigh -= 1) {
        decreaseScale(value_1, scaleHigh - scaleLow);
        setScale(value_1, scaleLow);
      }
    }
  }
}

int getSign(s21_decimal value) {
  return !!(value.bits[3] & (1u << 31));
}

s21_decimal *setSign(s21_decimal *value, int bit) {
  if (value) {
    if (bit) value->bits[3] |= (1u << 31);
    else value->bits[3] &= ~(1u << 31);
  }
  return value;
}

s21_decimal *convert(s21_decimal *value) {
  if (!value) return value;
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal add = {{1, 0, 0, 0}};
  
  for (int x = 0; x < 3; x += 1) value->bits[x] = ~value->bits[x];
  addBit(*value, add, &result);
  for (int x = 0; x < 3; x += 1) value->bits[x] = result.bits[x];
  
  setBit(value, 127, 1); // ИСПРАВЛЕНО: 127-й бит отвечает за знак минус
  return value;
}

int isNull(s21_decimal value) {
  return !value.bits[0] && !value.bits[1] && !value.bits[2];
}

int getFloatExp(float *value) {
  if (!value) return 0;
  unsigned int bits = *((unsigned int *)value);
  return ((bits & 0x7F800000) >> 23) - 127;
}
s21_decimal *leftShift(s21_decimal *value, int shift) {
  if (!value) return value;
  
  for (int y = 0; y < shift; y += 1) {
    int transfer_0_to_1 = !!(value->bits[0] & (1u << 31));
    int transfer_1_to_2 = !!(value->bits[1] & (1u << 31));
    
    value->bits[0] <<= 1;
    value->bits[1] <<= 1;
    value->bits[2] <<= 1;
    
    if (transfer_0_to_1) value->bits[1] |= 1u;
    if (transfer_1_to_2) value->bits[2] |= 1u;
  }
  return value;
}


int s21_is_correct_decimal(s21_decimal decimal) {
    int code = 1;

    if (s21_decimal_get_empty1(decimal) != 0 || s21_decimal_get_empty2(decimal) != 0) {
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

s21_decimal s21_decimal_get_inf(void) {
    s21_decimal result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF}};

    return result;
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
