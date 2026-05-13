#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result)
{
	  int is_correct = 0;
  if (!is_dec_correct(value_1) || !is_dec_correct(value_2) || !result)
  {
	  return 1;
  }
else {
    init_dec(result);

    if (isnull(value_2))
      *result = value_1;
    else if (isnull(value_1))
      *result = value_2;
    else {
      int left = abs(unidec(&value_1, &value_2));
      get(result, sign) = (mequal(value_1, value_2) == 1) ? get(&value_1, sign)
                                                          : get(&value_2, sign);

      if (get(&value_1, sign) == get(&value_2, sign))
        is_correct += add_dev(value_1, value_2, result, &left);
      else {
        if (mequal(value_1, value_2) == 1)
          invert(&value_1);
        else
          invert(&value_2);

        result->bits[3] ^= (1 << 31);
        get(&value_1, sign) = get(&value_2, sign) = get(result, sign);

        is_correct += add_dev(value_1, value_2, result, &left);

        invert(result);
        left = -left;
      }
      is_correct += bank_round(result, left);
    }
  }
  is_correct = is_correct ? get(result, sign) + 1 : is_correct;
  get(result, sign) &= !isnull(*result);

  return is_correct;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result)
{
  s21_negate(value_2, &value_2);
  return s21_add(value_1, value_2, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result)
{

  if (!is_dec_correct(value_1) || !is_dec_correct(value_2) || !result)
  {
	  return 1;
  }
  int is_correct = 0;
  init_dec(result);
  if (!isnull(value_1) && !isnull(value_2)) {
    s21_decimal leftovers = {{0}};

    get(result, expo) = get(&value_1, expo) + get(&value_2, expo);
    get(result, sign) = get(&value_1, sign) ^ get(&value_2, sign);

    is_correct = double_d_mul(value_1, value_2, result, &leftovers);

    int carry = 0;
    is_correct += merge_dev(result, leftovers, &carry);
    is_correct += bank_round(result, carry);

    if (is_correct) is_correct = get(result, sign) + 1;
  }
  return is_correct;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result)
{
  if (!is_dec_correct(value_1) || !is_dec_correct(value_2) || !result){
	  return 1;
  }

  init_dec(result);

  if (isnull(value_2)) return 3;
  if (isnull(value_1)) return 0;

  int is_correct = 0;
  prepare_div(&value_1, &value_2);

  int result_expo = (int)get(&value_1, expo) - (int)get(&value_2, expo);
  short result_sign = get(&value_1, sign) ^ get(&value_2, sign);

  int carry = 0;
  s21_decimal leftovers = {{0}};
  while (!isnull(value_1) && !is_correct) {
    if (mequal(value_1, value_2) < 0) {
      s21_decimal tmp_dec = leftovers;
      if (mul_by_ten(&tmp_dec))
        init_dec(&value_1);
      else {
        leftovers = tmp_dec;
        madd(leftovers, (s21_decimal){{mul_by_ten(result), 0, 0, 0}},
             &leftovers);
        result_expo++;
      }
      carry = mul_by_ten(&value_1);
    }
    while (((mequal(value_1, value_2) >= 0) || (carry > 0))) {
      carry -= msub(value_1, value_2, &value_1);

      if (madd(*result, (s21_decimal){{1, 0, 0, 0}}, result)) {
        is_correct += madd(leftovers, (s21_decimal){{1, 0, 0, 0}}, &leftovers);
        init_dec(result);
      }
    }
  }

  while (isnull(leftovers) && !is_correct && result_expo < 0) {
    is_correct += raiseexpo(result);
    result_expo++;
  }

  get(result, sign) = result_sign;

  if (!is_correct) {
    get(result, expo) = result_expo;

    is_correct = merge_dev(result, leftovers, &carry);
    is_correct += bank_round(result, carry);
  }

  if (is_correct) {
    result->bits[0] = result->bits[1] = result->bits[2] = UINT_MAX;

    get(result, expo) = 0;
    is_correct = get(result, sign) + 1;
  }

  return is_correct;
}
