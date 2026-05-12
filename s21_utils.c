#include "s21_decimal.h"

#include "s21_utils.h"

int is_dec_correct(s21_decimal dec) {
  return !get(&dec, zero1) && !get(&dec, zero2) && get(&dec, expo) <= 28;
}

void init_dec(s21_decimal *dec) {
  dec->bits[0] = 0;
  dec->bits[1] = 0;
  dec->bits[2] = 0;
  dec->bits[3] = 0;
}

int isnull(s21_decimal dec) {
  return dec.bits[0] == 0 && dec.bits[1] == 0 && dec.bits[2] == 0;
}

int unidec(s21_decimal *dec1, s21_decimal *dec2) {
  int tmp, carry = 0;
  if (isnull(*dec1)) get(dec1, expo) = 0;
  if (isnull(*dec2)) get(dec2, expo) = 0;
  if (isnull(*dec1)) get(dec1, expo) = get(dec2, expo);
  if (isnull(*dec2)) get(dec2, expo) = get(dec1, expo);
  while (get(dec1, expo) < get(dec2, expo) && !raiseexpo(dec1));
  while (get(dec1, expo) > get(dec2, expo) && !raiseexpo(dec2));
  while (!isnull(*dec1) && !isnull(*dec2) &&
         get(dec1, expo) < get(dec2, expo) && (tmp = downexpo(dec2)) != -1)
    carry = tmp;
  while (!isnull(*dec1) && !isnull(*dec2) &&
         get(dec1, expo) > get(dec2, expo) && (tmp = downexpo(dec1)) != -1)
    carry = -tmp;
  return carry;
}

int madd(s21_decimal val1, s21_decimal val2, s21_decimal *result) {
  int ret = 0;
  result->bits[0] = val1.bits[0] + val2.bits[0];
  if (result->bits[0] < val2.bits[0] || result->bits[0] < val1.bits[0]) {
    val1.bits[1]++;
    if (val1.bits[1] == 0) {
      val1.bits[2]++;
      if (val1.bits[2] == 0) ret = 1;
    }
  }
  result->bits[1] = val1.bits[1] + val2.bits[1];
  if (result->bits[1] < val2.bits[1] || result->bits[1] < val1.bits[1]) {
    val1.bits[2]++;
    if (val1.bits[2] == 0) ret = 1;
  }
  result->bits[2] = val1.bits[2] + val2.bits[2];
  if (result->bits[2] < val2.bits[2] || result->bits[2] < val1.bits[2]) ret = 1;

  return ret;
}

int add_dev(s21_decimal val1, s21_decimal val2, s21_decimal *result,
            int *left) {
  int ret = 0;
  /*пока мантисса не будет умещаться в 96 бит сдвигаем экспоненту*/
  while (ret == 0 && (ret = madd(val1, val2, result))) {
    /*если экспоненту еще есть куда сдвигать*/
    if (get(&val1, expo) && get(&val2, expo)) {
      *left = downexpo(&val1) + downexpo(&val2);
      /*если сумма остатков от деления мантисс на 10 больше 9, то
       * добавляем единицу к одной из мантисс.Переполнение тут невозможно,
       * т.к. мантисса только что поделена на 10. */
      if (*left > 9) madd(val1, ONE_DEC, &val1);
      *left %= 10;
      ret = 0;
    }
  }
  get(result, expo) = get(&val1, expo);
  /*Нормализуем экспоненту*/
  while (get(result, expo) > 28) *left = downexpo(result);
  return ret;
}

int msub(s21_decimal val1, s21_decimal val2, s21_decimal *result) {
  int ret = 0;
  result->bits[0] = val1.bits[0] - val2.bits[0];
  if (val2.bits[0] > val1.bits[0]) {
    val1.bits[1]--;
    if (val1.bits[1] == UINT_MAX) {
      val1.bits[2]--;
      if (val1.bits[2] == UINT_MAX) ret = 1;
    }
  }
  result->bits[1] = val1.bits[1] - val2.bits[1];
  if (val2.bits[1] > val1.bits[1]) {
    val1.bits[2]--;
    if (val1.bits[2] == UINT_MAX) ret = 1;
  }
  result->bits[2] = val1.bits[2] - val2.bits[2];
  if (val2.bits[2] > val1.bits[2]) ret = 1;
  return ret;
}

int downexpo_extra(s21_decimal *dec, unsigned left) {
  /*временная переменная двойного объема для переноса остатка между разрядами*/
  unsigned long tmp = 0UL + left;
  if (!isnull(*dec) || left)
    for (int n = 2; n >= 0; n--) {
      /*переносим остаток в низлежащий разряд*/
      tmp = tmp * (UINT_MAX + 1UL) + dec->bits[n];
      /*делим текущий разряд на 10*/
      dec->bits[n] = (unsigned int)(tmp / 10);
      /*сохраняем остаток от деления для следующего разряда*/
      tmp = tmp % 10;
    }
  get(dec, expo)--;
  return (int)tmp;
}

int downexpo(s21_decimal *dec) {
  /*Экспонента предельная - дальше сокращать нельзя*/
  if (get(dec, expo) == 0) return -1;
  return downexpo_extra(dec, 0);
}

int raiseexpo(s21_decimal *dec) {
  if (get(dec, expo) >= 28U) return 1;
  s21_decimal tmp = *dec;
  int error = mul_by_ten(&tmp);
  /*в результате сдвига или при переполнении разрядов значимые цифры вывалились
   * за левый край. значит сокращение нулей невозможно*/
  if (!error) {
    /*переполнения мантиссы не произошло, можно записать результат умножения и
     * увеличить экспоненту без потери точности. */
    dec->bits[0] = tmp.bits[0];
    dec->bits[1] = tmp.bits[1];
    dec->bits[2] = tmp.bits[2];
    get(dec, expo)++;
  }
  return error;
}

unsigned int mul_by_ten(s21_decimal *dec) {
  unsigned ret = 0;
  unsigned long tmp = 0;
  for (int n = 0; n < 3; n++) {
    tmp += 10UL * (unsigned long)dec->bits[n];
    ret = tmp >> 32;
    dec->bits[n] = (unsigned int)(tmp % (UINT_MAX + 1UL));
    tmp /= (unsigned long)UINT_MAX + 1UL;
  }
  return ret;
}

void invert(s21_decimal *dec) {
  for (int i = 0; i < 3; i++) {
    dec->bits[i] = ~dec->bits[i];
  }
  dec->bits[3] ^= 1 << 31;
}

int mequal(s21_decimal dec1, s21_decimal dec2) {
  int ret = 0;
  for (int i = 2; i >= 0 && !ret; i--)
    ret = dec1.bits[i] > dec2.bits[i]   ? 1
          : dec1.bits[i] < dec2.bits[i] ? -1
                                        : 0;

  return ret;
}

#ifdef DEBUG
void print_dec(s21_decimal dec) {
  short start = 0;
  if (get(&dec, sign) == 1)
    printf("-");
  else
    printf(" ");
  for (int i = 95; i >= 0; i--) {
    if (get_bit(&dec, i)) {
      start = 1;
    }
    printf("%d", get_bit(&dec, i));
    if (i % 32 == 0) printf(" ");
  }
  if (start == 0) printf("0");
  if (get(&dec, expo) > 0) printf("E-%d", get(&dec, expo));
  printf("\n");
}
#endif

int bank_round(s21_decimal *dec, int digit) {
  int ret = 0;
  if (abs(digit) > 5 || (abs(digit) == 5 && get_bit(dec, 0))) {
    if (digit < 0)
      msub(*dec, (s21_decimal){{1, 0, 0, 0}}, dec);
    else if (madd(*dec, (s21_decimal){{1, 0, 0, 0}}, dec)) {
      *dec = (s21_decimal){{0, 0, 0, dec->bits[3]}};
      if (get(dec, expo) > 0)
        downexpo_extra(dec, 1), madd(*dec, (s21_decimal){{1, 0, 0, 0}}, dec);
      else
        ret = get(dec, sign) + 1;
    }
  }
  return ret;
}

int merge_dev(s21_decimal *dec, s21_decimal leftovers, int *left) {
  int ret = 0;
  while (!isnull(leftovers) && !ret) {
    /* если экспоенента результата уже 0 то переполнение добавлять некуда, у
     * нас ошибка переполнения*/
    ret += (get(dec, expo) == 0);
    if (!ret) {
      /*уменьшаем экспоненту переполнения поделив на 10*/
      *left = downexpo_extra(&leftovers, 0);
      /* уменьшаем экспоненту результата делением на 10 и добавляем остатки от
       * деления переполнения в старший элемент результата */
      *left = downexpo_extra(dec, *left);
    }
  }
  /*Нормализуем экспоненту*/
  while (get(dec, expo) > 28) *left = downexpo(dec);
  return ret;
}

void prepare_div(s21_decimal *value_1, s21_decimal *value_2) {
  /*увеличиваем экспоненту делимого и делителя до максимума (пока влезают
   * разряды)*/
  unsigned tmp_exp = get(value_2, expo);
  get(value_2, expo) = 0;
  while (!raiseexpo(value_2)) tmp_exp += get(value_2, expo)--;
  get(value_2, expo) += tmp_exp;
  tmp_exp = get(value_1, expo);
  get(value_1, expo) = 0;
  while (!raiseexpo(value_1)) tmp_exp += get(value_1, expo)--;
  get(value_1, expo) += tmp_exp;
  /*уменьшаем экспоненту делителя до минимума или чтобы он не был больше
   * делимого*/
  tmp_exp = get(value_2, expo);
  get(value_2, expo) = 28U;
  s21_decimal tmp_dec = *value_2;
  while ((mequal(*value_1, *value_2) < 0) && downexpo(&tmp_dec) == 0)
    *value_2 = tmp_dec;
  /*тут экспоенета может стать отрицательной, по сему будем увеличивать
   * экспоненты обоих аргументов, пока экспонента делителя не станет хотя бы
   * нулевой*/
  while (28U - get(value_2, expo) > tmp_exp)
    get(value_2, expo)++, get(value_1, expo)++;
  get(value_2, expo) = tmp_exp - (28U - get(value_2, expo));
  /*сокращаем экспоненты*/
  while (get(value_1, expo) > 0 && get(value_2, expo) > 0)
    get(value_1, expo)--, get(value_2, expo)--;
}

int double_d_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                 s21_decimal *leftovers) {
  int ret = 0;
  /* перемножаем все элементы мантисс множителей, как с матрицей */
  for (int i = 0; i <= 2 && !ret; i++) {
    /* переменная для переполнения элементов при умножении */
    unsigned long carry = 0;
    short lo_index = 0;
    for (int j = 0; j < 3; j++) {
      /* перемножаем биты мантиссы */
      unsigned long r =
          (unsigned long)value_1.bits[i] * (unsigned long)value_2.bits[j];
      /* добавляем остатки с прошлого круга */
      r += carry;
      /* переносим старшие биты в остатки на следующий круг */
      carry = r >> 32;
      /* обрезаем старшие биты */
      r &= UINT_MAX;
      /* заполняем элементы мантиссы результата с переносом переполнения в
       * старший элемент, если есть*/
      if (j + i <= 2)
        r += result->bits[j + i], result->bits[i + j] = (unsigned)r;
      /* если нет, то добавляем в децимал остатков */
      else
        r += leftovers->bits[lo_index],
            leftovers->bits[lo_index++] = (unsigned)r;
      /* добавляем в остатки на следующий круг добавившееся переполнение -
       * старшие биты*/
      carry += r >> 32;
      /* после перемножения последнего элемента второго множителя остатки уже
       * идут в децимал переполнения*/
      if (carry && j == 2) {
        /* чтобы попасть в нужный элемент мантиссы переполнения создаем
         * временный децимал*/
        s21_decimal tmp_dec = {{0}};
        tmp_dec.bits[i] = (unsigned)carry;
        /* если переполнится переполнения, то никакой жкспоненты не хватит,
         * можно дальше не играть*/
        ret += madd(*leftovers, tmp_dec, leftovers);
      }
    }
  }
  return ret;
}
