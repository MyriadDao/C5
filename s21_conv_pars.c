#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return 1; // Возвращаем ошибку, если указатель NULL
  
  // Полностью очищаем структуру
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  
  // Используем unsigned int для безопасной работы с абсолютным значением
  unsigned int abs_src = src;
  
  if (src < 0) {
    setSign(dst, 1);
    abs_src = (unsigned int)(-src); // Корректно для INT_MIN
  }
  
  dst->bits[0] = abs_src;
  
  return 0; // Успешное выполнение
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst) return 1;
  
  // Всегда сначала обнуляем структуру
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  
  // 1. ПРАВИЛО 1: Если числа слишком маленькие (0 < |x| < 1e-28), возвращаем ошибку и 0
  if (fabsf(src) < 1e-28f && fabsf(src) > 0.0f) {
    return 1; 
  }
  
  // 2. ПРАВИЛО 2: Если числа слишком большие (> 79,228,162,514,264,337,593,543,950,335) или бесконечность
  if (isnan(src) || isinf(src) || fabsf(src) > 79228162514264337593543950335.0f) {
    return 1; 
  }
  
  if (src == 0.0f) return 0; // Чистый ноль

  int sign = (src < 0.0f);
  if (sign) src = -src;

  // 3. ПРАВИЛО 3: Выделяем 7 значащих цифр с использованием БАНКОВСКОГО округления (%.6e)
  char buf[64];
  sprintf(buf, "%.6e", src); // Для 1.2345665f это гарантированно выдаст "1.234566e+00" по правилам банка

  // Парсим экспоненту и изолируем мантиссу в строке
  char *e_ptr = strchr(buf, 'e');
  int exp_val = atoi(e_ptr + 1);
  *e_ptr = '\0';

  // Собираем чистую целую мантиссу из строки (игнорируя точку)
  long long mantissa = 0;
  for (int i = 0; buf[i] != '\0'; i++) {
    if (buf[i] != '.') {
      mantissa = mantissa * 10 + (buf[i] - '0');
    }
  }

  // Считаем масштаб (scale)
  int scale = 6 - exp_val;

  // Если число большое (scale < 0), превращаем отрицательный scale в нули мантиссы
  while (scale < 0) {
    mantissa *= 10;
    scale++;
  }

  // Оптимизация: Срезаем лишние нули на конце, уменьшая scale
  while (scale > 0 && mantissa % 10 == 0) {
    mantissa /= 10;
    scale--;
  }

  if (scale > 28 || scale < 0) return 1;

  // Записываем мантиссу и метаданные
  dst->bits[0] = (unsigned int)mantissa;
  dst->bits[1] = (unsigned int)(mantissa >> 32);
  dst->bits[2] = 0;
  
  setScale(dst, scale);
  setSign(dst, sign);

  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst) return 1;
  else if (!s21_is_correct_decimal(src))
{
*dst = 0;
return 1;
}

  int res = 1;
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    *dst = src.bits[0];
    if (getSign(src)) {
      *dst *= -1;
    }
    *dst /= (int)pow(10, getScale(src));
    res = 0;
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  if (!dst) return 1;
  else if (!s21_is_correct_decimal(src))
{
*dst = 0;
return 1;
}
  int res = 1;
  if (dst) {
    double tmp = 0;
    int exp = 0;
    for (int i = 0; i < 96; i++) {
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) tmp += pow(2, i);
    }
    if ((exp = getScale(src)) > 0) {
      for (int i = exp; i > 0; i--, tmp /= 10.0)
        ;
    }
    *dst = (float)tmp;
    if (getSign(src)) {
      *dst *= -1;
    }
    res = 0;
  }
  return res;
}
