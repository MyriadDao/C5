#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2)
{
	return s21_is_greater(value_2, value_1);
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2)
{
	return (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2)
{
	  int ret = 0, flag = 0;

  if (get(&value_1, sign) != get(&value_2, sign)) {
    if (isnull(value_1) && isnull(value_2))
      ret = 0;
    else
      ret = get(&value_1, sign) == 0 ? 1 : 0;
  } else {
    if (get(&value_1, expo) != get(&value_2, expo)) {
      ret = unidec(&value_1, &value_2);
      ret = 0;
    }

    for (int i = 95; i >= 0; i--) {
      if ((get_bit(&value_1, i) != get_bit(&value_2, i)) && flag == 0) {
        flag = 1;

        if (get_bit(&value_1, i) > get_bit(&value_2, i))
          ret = get(&value_1, sign) == 0 ? 1 : 0;
        else
          ret = get(&value_1, sign) == 0 ? 0 : 1;
      }
    }
  }
  return ret;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2)
{
	return (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2));
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2)
{
	int ret = 1;
  if (get(&value_1, sign) == get(&value_2, sign)) {
    unidec(&value_1, &value_2);

    for (int i = 95; i >= 0 && ret; i--) {
      if (get_bit(&value_1, i) != get_bit(&value_2, i)) ret = 0;
    }

  } else {
    ret = (isnull(value_1) && isnull(value_2)) ? 1 : 0;
  }
  return ret;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2)
{
	return !s21_is_equal(value_1, value_2);
}
