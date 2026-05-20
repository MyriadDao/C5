#include "s21_decimal.h"

static int s21_get_last_bit(s21_decimal value) {
  int bitLast = 95;
  for (; (bitLast >= 0) && (!s21_get_bit(value, bitLast));) {
    bitLast -= 1;
  }
  return bitLast;
}

static void s21_sub_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int borrow = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = s21_get_bit(value_1, i);
    int bit2 = s21_get_bit(value_2, i);
    
    int diff = bit1 - bit2 - borrow;
    if (diff < 0) {
      diff += 2;
      borrow = 1;
    } else {
      borrow = 0;
    }
    s21_set_bit(result, i, diff);
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }

  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  int valueSign_1 = s21_decimal_get_sign(value_1);
  int valueSign_2 = s21_decimal_get_sign(value_2);
  int status = 0;

  if (valueSign_1 == valueSign_2) {
    s21_alignment_scale(&value_1, &value_2);

    while (s21_add_bit(value_1, value_2, result)) {
      int current_scale = s21_decimal_get_power(value_1);
      
      if (current_scale == 0) {
        status = valueSign_1 ? 2 : 1; // 1 - слишком большое, 2 - слишком маленькое
        break;
      }
      
      s21_decrease_scale(&value_1, 1);
      s21_decrease_scale(&value_2, 1);
      
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
    }

    if (status == 0) {
      s21_set_scale(result, s21_decimal_get_power(value_1));
      s21_set_sign(result, valueSign_1);
    }

  } else {
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    s21_set_sign(&abs_1, 0);
    s21_set_sign(&abs_2, 0);

    if (valueSign_1 && !valueSign_2) {
      status = s21_sub(abs_2, abs_1, result);
    } else if (!valueSign_1 && valueSign_2) {
      status = s21_sub(abs_1, abs_2, result);
    }
  }

  return status;
}


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return 1;

    if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }
  
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  int valueSign_1 = s21_decimal_get_sign(value_1);
  int valueSign_2 = s21_decimal_get_sign(value_2);
  int status = 0;

  if (valueSign_1 != valueSign_2) {
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    s21_set_sign(&abs_1, 0);
    s21_set_sign(&abs_2, 0);
    
    status = s21_add(abs_1, abs_2, result);
    
    if (status) {
      status = valueSign_1 ? 2 : 1;
    } else {
      s21_set_sign(result, valueSign_1);
    }
    
  } else {
    s21_alignment_scale(&value_1, &value_2);
    
    int final_scale = s21_decimal_get_power(value_1);
    
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    s21_set_sign(&abs_1, 0);
    s21_set_sign(&abs_2, 0);
    
    if (s21_is_equal(abs_1, abs_2)) {
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
      s21_set_scale(result, final_scale);
    } else if (s21_is_less(abs_1, abs_2)) {
      s21_sub_bit(abs_2, abs_1, result);
      s21_set_scale(result, final_scale);
      s21_set_sign(result, !valueSign_1);
    } else {
      s21_sub_bit(abs_1, abs_2, result);
      s21_set_scale(result, final_scale);
      s21_set_sign(result, valueSign_1);
    }
  }
  
  return status;
}


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }
  
  for (int x = 0; x < 4; x++) result->bits[x] = 0;
  int sign_result = (s21_decimal_get_sign(value_1) != s21_decimal_get_sign(value_2));
  int scale_result = s21_decimal_get_power(value_1) + s21_decimal_get_power(value_2);
  
  s21_set_sign(&value_1, 0); s21_set_scale(&value_1, 0);
  s21_set_sign(&value_2, 0); s21_set_scale(&value_2, 0);

  s21_decimal tmp_result = {{0, 0, 0, 0}};
  int overflow = 0;

  int bit1 = s21_get_last_bit(value_1);
  for (int i = 0; i <= bit1; i++) {
    if (s21_get_bit(value_1, i)) {
      s21_decimal shifted_v2 = value_2;
      
      if (s21_get_last_bit(shifted_v2) + i > 95) {
        overflow = 1;
        break;
      }
      s21_left_shift(&shifted_v2, i);
      
      if (s21_add_bit(tmp_result, shifted_v2, &tmp_result)) {
        overflow = 1;
        break;
      }
    }
  }

  while (overflow && scale_result > 0) {
    overflow = 0;
    if (s21_decimal_get_power(value_1) > 0) s21_decrease_scale(&value_1, 1);
    else if (s21_decimal_get_power(value_2) > 0) s21_decrease_scale(&value_2, 1);
    scale_result--;
    
    for (int x = 0; x < 4; x++) tmp_result.bits[x] = 0;
    int b1 = s21_get_last_bit(value_1);
    for (int i = 0; i <= b1; i++) {
      if (s21_get_bit(value_1, i)) {
        s21_decimal shifted = value_2;
        if (s21_get_last_bit(shifted) + i > 95 || s21_add_bit(tmp_result, *s21_left_shift(&shifted, i), &tmp_result)) {
          overflow = 1;
          break;
        }
      }
    }
  }

  int status = 0;
  if (overflow || scale_result > 28) {
    status = sign_result ? 2 : 1; 
  } else {
    *result = tmp_result;
    s21_set_scale(result, scale_result);
    s21_set_sign(result, sign_result);
  }

  return status;
}


int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 4;
  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) return 4;
  
  for (int x = 0; x < 4; x++) result->bits[x] = 0;
  if (s21_is_null(value_2)) return 3;
  if (s21_is_null(value_1)) return 0;

  int sign_result = (s21_decimal_get_sign(value_1) != s21_decimal_get_sign(value_2));
  int scale_1 = s21_decimal_get_power(value_1);
  int scale_2 = s21_decimal_get_power(value_2);
  
  s21_set_sign(&value_1, 0); s21_set_scale(&value_1, 0);
  s21_set_sign(&value_2, 0); s21_set_scale(&value_2, 0);

  s21_decimal quotient = {{0, 0, 0, 0}};
  s21_decimal remainder = {{0, 0, 0, 0}};
  int final_scale = scale_1 - scale_2;

  int status = 0;
  int digits_count = 0;

  while (digits_count < 30) {
    for (int i = 95; i >= 0; i--) {
      s21_left_shift(&remainder, 1);
      s21_set_bit(&remainder, 0, s21_get_bit(value_1, i));
      
      s21_left_shift(&quotient, 1);
      if (s21_is_greater_or_equal(remainder, value_2)) {
        s21_sub(remainder, value_2, &remainder);
        s21_set_bit(&quotient, 0, 1);
      }
    }

    if (s21_is_null(remainder)) {
      break;
    }

    s21_decimal q_x2 = quotient;
    s21_decimal q_x8 = quotient;
    s21_left_shift(&q_x2, 1);
    s21_left_shift(&q_x8, 3);
    
    s21_decimal next_quotient = {{0, 0, 0, 0}};
    if (s21_add_bit(q_x2, q_x8, &next_quotient)) {
      break;
    }
    quotient = next_quotient;

    s21_decimal rem_x2 = remainder;
    s21_decimal rem_x8 = remainder;
    s21_left_shift(&rem_x2, 1);
    s21_left_shift(&rem_x8, 3);
    s21_add_bit(rem_x2, rem_x8, &value_1); 

    final_scale++;
    digits_count++;
    
    if (final_scale > 40) { 
      break;
    }
  }

  while (final_scale < 0) {
    s21_decimal check = quotient;
    s21_increase_scale(&check, 1);
    if (s21_decimal_get_power(check) > s21_decimal_get_power(quotient)) {
      quotient = check;
      final_scale++;
    } else {
      status = sign_result ? 2 : 1;
      break;
    }
  }

  while (final_scale > 28) {
    s21_decrease_scale(&quotient, 1);
    final_scale--;
  }

  if (status == 0) {
    if (s21_is_null(quotient)) {
      status = 2;
    } else {
      *result = quotient;
      s21_set_scale(result, final_scale);
      s21_set_sign(result, sign_result);
    }
  }

  return status;
}
