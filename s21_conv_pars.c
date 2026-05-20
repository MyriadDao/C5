#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return 1; 
  
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  
  unsigned int abs_src = src;
  
  if (src < 0) {
    s21_set_sign(dst, 1);
    abs_src = (unsigned int)(-src); 
  }
  
  dst->bits[0] = abs_src;
  
  return s21_is_correct_decimal(*dst); 
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst) return 1;
  
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  
  if (fabsf(src) < 1e-28f && fabsf(src) > 0.0f) {
    return 1; 
  }
  
  if (isnan(src) || isinf(src) || fabsf(src) > 79228162514264337593543950335.0f) {
    return 1; 
  }
  
  if (src == 0.0f) return 0;

  int sign = (src < 0.0f);
  if (sign) src = -src;

  char buf[64];
  sprintf(buf, "%.6e", src); 

  char *e_ptr = strchr(buf, 'e');
  int exp_val = atoi(e_ptr + 1);
  *e_ptr = '\0';

  long long mantissa = 0;
  for (int i = 0; buf[i] != '\0'; i++) {
    if (buf[i] != '.') {
      mantissa = mantissa * 10 + (buf[i] - '0');
    }
  }

  int scale = 6 - exp_val;

  while (scale < 0) {
    mantissa *= 10;
    scale++;
  }

  while (scale > 0 && mantissa % 10 == 0) {
    mantissa /= 10;
    scale--;
  }

  if (scale > 28 || scale < 0) return 1;

  dst->bits[0] = (unsigned int)mantissa;
  dst->bits[1] = (unsigned int)(mantissa >> 32);
  dst->bits[2] = 0;
  
  s21_set_scale(dst, scale);
  s21_set_sign(dst, sign);

  return s21_is_correct_decimal(*dst); 
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
    if (s21_decimal_get_sign(src)) {
      *dst *= -1;
    }
    *dst /= (int)pow(10, s21_decimal_get_power(src));
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
    if ((exp = s21_decimal_get_power(src)) > 0) {
      for (int i = exp; i > 0; i--, tmp /= 10.0)
        ;
    }
    *dst = (float)tmp;
    if (s21_decimal_get_sign(src)) {
      *dst *= -1;
    }
    res = 0;

  }
  return res;
  }
