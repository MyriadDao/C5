#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  else if (!s21_is_correct_decimal(value)) return 1;

  // Шаг 1. Вызываем наш обновленный truncate
  int status = s21_truncate(value, result);
  if (status != 0) return 1;

  int sign = !!(value.bits[3] & (1u << 31));

  // Шаг 2. Проверяем, нужно ли уменьшать число дальше 
  // (только для ОТРИЦАТЕЛЬНЫХ чисел с дробной частью)
  if (sign) {
    // Проверяем, изменилось ли число после truncate (была ли дробная часть)
    int has_fraction = 0;
    s21_decimal check_val;
    s21_truncate(value, &check_val); // повторно для изоляции
    
    // Если оригинальное число не равно усеченному — значит дробь была
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
  if (!result) return 1;

  // 1. Полностью зануляем результат
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;

  int scale = (value.bits[3] >> 16) & 0xFF;
  int sign = !!(value.bits[3] & (1u << 31));

  // Копируем мантиссу во внутренний массив СТРОГО беззнаковых интов
  unsigned int mantissa[3];
  mantissa[0] = value.bits[0];
  mantissa[1] = value.bits[1];
  mantissa[2] = value.bits[2];

  // 2. Цикл деления на 10
  for (int y = 0; y < scale; y++) {
    unsigned long long remainder = 0;
    
    // Строго сверху вниз, используя исключительно unsigned long long
    for (int x = 2; x >= 0; x--) {
      unsigned long long current = (remainder << 32) + mantissa[x];
      mantissa[x] = (unsigned int)(current / 10);
      remainder = current % 10;
    }
  }

  // 3. Записываем обратно в результат
  result->bits[0] = mantissa[0];
  result->bits[1] = mantissa[1];
  result->bits[2] = mantissa[2];

  // Выставляем знак (масштаб после truncate всегда 0)
  if (sign) {
    result->bits[3] |= (1u << 31);
  }

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
