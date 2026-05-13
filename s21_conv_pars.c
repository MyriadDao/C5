#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal* dest)
{
	  int code = CALCULATION_ERROR;
  if (dest) {
    code = SUCCESS;
    *dest = ZERO_DEC;
    dest->bits[0] = (unsigned)abs(src);
    get(dest, sign) = src < 0;
  }
  return code;
}

int s21_from_float_to_decimal(float src, s21_decimal* dest)
{
	  int code = CALCULATION_ERROR;
  if (dest && isfinite(src)) {
    *dest = ZERO_DEC;
    code = SUCCESS;
    if (src != 0) {
      s21_decimal tmp = ZERO_DEC;
      binaryfloat *bsrc = (binaryfloat *)&src;
      get(dest, sign) = get(&tmp, sign) = bsrc->sign;

      int e = bsrc->expo - 127;
      for (int i = 0; i <= 23 && code == SUCCESS; i++) {
        if (i == 0 || (bsrc->mantisse & (1 << (23 - i)))) {
          if (e - i > 95)
            code = CALCULATION_ERROR;
          else if (e - i >= 0)
            set_bit(dest, e - i);
          else if (e - i >= -96)
            set_bit(&tmp, 96 + (e - i));
        }
      }
      while (code == SUCCESS && get(dest, expo) < 28 && !isnull(tmp)) {
        code += raiseexpo(dest);
        madd(*dest, (s21_decimal){{mul_by_ten(&tmp), 0, 0, 0}}, dest);
      }
    }
  }
  return code;
}

int s21_from_decimal_to_int(s21_decimal src, int* dest)
{
	  int code = CALCULATION_ERROR;
  if (dest) {
    s21_truncate(src, &src);
    if (src.bits[1] == 0 && src.bits[2] == 0 &&
        src.bits[0] <= (unsigned)INT_MAX + get(&src, sign)) {
      code = SUCCESS;
      *dest = (int)(long int)(src.bits[0]) * (get(&src, sign) ? -1L : 1L);
    }
  }
  return code;
}

int s21_from_decimal_to_float(s21_decimal src, float* dest)
{
	  int code = CALCULATION_ERROR;
  if (dest) {
    *dest = 0;
    code = SUCCESS;
    if (!isnull(src)) {
      binaryfloat *bdst = (binaryfloat *)dest;
      bdst->expo = 127 + 95;
      bdst->sign = get(&src, sign);

      while (bdst->expo >= 127 && !get_bit(&src, bdst->expo - 127)) {
        bdst->expo--;
      }

      for (int i = 0; i < 23; i++) {
        int b = bdst->expo - i - 128;
        if (b >= 0) bdst->mantisse |= get_bit(&src, b) << (22 - i);
      }

      *dest /= pow(10, get(&src, expo));
    }
  }
  return code;
}
