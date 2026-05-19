#include "s21_decimal.h"

static void s21_sub_bit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int borrow = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = getBit(value_1, i);
    int bit2 = getBit(value_2, i);
    
    int diff = bit1 - bit2 - borrow;
    if (diff < 0) {
      diff += 2;
      borrow = 1;
    } else {
      borrow = 0;
    }
    setBit(result, i, diff);
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }

  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  int valueSign_1 = getSign(value_1);
  int valueSign_2 = getSign(value_2);
  int status = 0;

  if (valueSign_1 == valueSign_2) {
    alignmentScale(&value_1, &value_2);

    while (addBit(value_1, value_2, result)) {
      int current_scale = getScale(value_1);
      
      if (current_scale == 0) {
        status = valueSign_1 ? 2 : 1; // 1 - слишком большое, 2 - слишком маленькое
        break;
      }
      
      decreaseScale(&value_1, 1);
      decreaseScale(&value_2, 1);
      
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
    }

    if (status == 0) {
      setScale(result, getScale(value_1));
      setSign(result, valueSign_1);
    }

  } else {
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);

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
  int valueSign_1 = getSign(value_1);
  int valueSign_2 = getSign(value_2);
  int status = 0;

  if (valueSign_1 != valueSign_2) {
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);
    
    status = s21_add(abs_1, abs_2, result);
    
    if (status) {
      status = valueSign_1 ? 2 : 1;
    } else {
      setSign(result, valueSign_1);
    }
    
  } else {
    alignmentScale(&value_1, &value_2);
    
    int final_scale = getScale(value_1);
    
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);
    
    if (s21_is_equal(abs_1, abs_2)) {
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
      setScale(result, final_scale);
    } else if (s21_is_less(abs_1, abs_2)) {
      s21_sub_bit(abs_2, abs_1, result);
      setScale(result, final_scale);
      setSign(result, !valueSign_1);
    } else {
      s21_sub_bit(abs_1, abs_2, result);
      setScale(result, final_scale);
      setSign(result, valueSign_1);
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
  int sign_result = (getSign(value_1) != getSign(value_2));
  int scale_result = getScale(value_1) + getScale(value_2);
  
  setSign(&value_1, 0); setScale(&value_1, 0);
  setSign(&value_2, 0); setScale(&value_2, 0);

  s21_decimal tmp_result = {{0, 0, 0, 0}};
  int overflow = 0;

  int bit1 = getBitLast(value_1);
  for (int i = 0; i <= bit1; i++) {
    if (getBit(value_1, i)) {
      s21_decimal shifted_v2 = value_2;
      
      if (getBitLast(shifted_v2) + i > 95) {
        overflow = 1;
        break;
      }
      leftShift(&shifted_v2, i);
      
      if (addBit(tmp_result, shifted_v2, &tmp_result)) {
        overflow = 1;
        break;
      }
    }
  }

  while (overflow && scale_result > 0) {
    overflow = 0;
    if (getScale(value_1) > 0) decreaseScale(&value_1, 1);
    else if (getScale(value_2) > 0) decreaseScale(&value_2, 1);
    scale_result--;
    
    for (int x = 0; x < 4; x++) tmp_result.bits[x] = 0;
    int b1 = getBitLast(value_1);
    for (int i = 0; i <= b1; i++) {
      if (getBit(value_1, i)) {
        s21_decimal shifted = value_2;
        if (getBitLast(shifted) + i > 95 || addBit(tmp_result, *leftShift(&shifted, i), &tmp_result)) {
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
    setScale(result, scale_result);
    setSign(result, sign_result);
  }

  return status;
}


int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
      if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }
  
  
  for (int x = 0; x < 4; x++) result->bits[x] = 0;
  
  if (isNull(value_2)) return 3; 
  if (isNull(value_1)) return 0; 

  int sign_result = (getSign(value_1) != getSign(value_2));
  int scale_1 = getScale(value_1);
  int scale_2 = getScale(value_2);
  
  setSign(&value_1, 0); setScale(&value_1, 0);
  setSign(&value_2, 0); setScale(&value_2, 0);

  s21_decimal quotient = {{0, 0, 0, 0}};
  s21_decimal remainder = {{0, 0, 0, 0}};
  s21_decimal temp_result = {{0, 0, 0, 0}};
  
  int final_scale = scale_1 - scale_2;
  int status = 0;

  for (int step = 0; step < 200; step++) { 
    
    for (int i = 95; i >= 0; i--) {
      leftShift(&remainder, 1);
      setBit(&remainder, 0, getBit(value_1, i));
      
      if (s21_is_greater_or_equal(remainder, value_2)) {
        s21_sub(remainder, value_2, &remainder);
        leftShift(&quotient, 1);
        setBit(&quotient, 0, 1);
      } else {
        leftShift(&quotient, 1);
      }
    }

    if (addBit(temp_result, quotient, &temp_result)) {
      status = sign_result ? 2 : 1;
      break;
    }

    if (isNull(remainder)) {
      break;
    }

    s21_decimal check_rem = remainder;
    increaseScale(&check_rem, 1);
    if (getScale(check_rem) == 0) {
      break;
    }
    
    value_1 = remainder; 
    increaseScale(&value_1, 1);
    setScale(&value_1, 0);
    
    final_scale++;
    for (int x = 0; x < 4; x++) quotient.bits[x] = 0;
  }

  if (status == 0) {
    while (final_scale < 0) {
      increaseScale(&temp_result, 1);
      final_scale++;
    }
    while (final_scale > 28) {
      decreaseScale(&temp_result, 1);
      final_scale--;
    }
    
    if (isNull(temp_result) && !isNull(value_1)) {
      status = 2; 
    } else {
      *result = temp_result;
      setScale(result, final_scale);
      setSign(result, sign_result);
    }
  }

  return status;
}
