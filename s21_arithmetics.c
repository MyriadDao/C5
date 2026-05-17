#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }

  // 1. Сброс результата и получение знаков
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  int valueSign_1 = getSign(value_1);
  int valueSign_2 = getSign(value_2);
  int status = 0;

  // СЦЕНАРИЙ 1: Знаки одинаковые (Сложение мантисс)
  if (valueSign_1 == valueSign_2) {
    // Выравниваем масштабы чисел
    alignmentScale(&value_1, &value_2);

    // Пытаемся сложить мантиссы
    while (addBit(value_1, value_2, result)) {
      int current_scale = getScale(value_1);
      
      // Если масштаб уже 0, уменьшать его нельзя — это жесткое переполнение
      if (current_scale == 0) {
        status = valueSign_1 ? 2 : 1; // 1 - слишком большое, 2 - слишком маленькое
        break;
      }
      
      // ИСПРАВЛЕНО: Безопасное уменьшение масштаба без рекурсии
      // decreaseScale внутри себя уже уменьшает масштаб на 1
      decreaseScale(&value_1, 1);
      decreaseScale(&value_2, 1);
      
      // Сбрасываем result перед следующей попыткой сложения в цикле
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
    }

    // Если переполнения не случилось, сохраняем масштаб и знак
    if (status == 0) {
      setScale(result, getScale(value_1));
      setSign(result, valueSign_1);
    }

  // СЦЕНАРИЙ 2: Знаки разные (Сводится к вычитанию мантисс)
  } else {
    // Создаем чистые копии для передачи в s21_sub
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);

    if (valueSign_1 && !valueSign_2) {
      // (-A) + (+B) => B - A
      status = s21_sub(abs_2, abs_1, result);
    } else if (!valueSign_1 && valueSign_2) {
      // (+A) + (-B) => A - B
      status = s21_sub(abs_1, abs_2, result);
    }
  }

  return status;
}


// Вспомогательная функция побитового вычитания мантисс (value_1 должно быть > value_2)
void subBit(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (!result) return 1;

    if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }
  
  // 1. Обнуляем результат и сохраняем исходные знаки
  for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
  int valueSign_1 = getSign(value_1);
  int valueSign_2 = getSign(value_2);
  int status = 0;

  // 2. ЕСЛИ ЗНАКИ РАЗНЫЕ: (+A) - (-B) или (-A) - (+B)
  // Это всегда сводится к сложению мантисс: |A| + |B|
  if (valueSign_1 != valueSign_2) {
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);
    
    status = s21_add(abs_1, abs_2, result);
    
    // Если сложение переполнилось, возвращаем код ошибки (1 - слишком большое, 2 - слишком маленькое)
    if (status) {
      status = valueSign_1 ? 2 : 1;
    } else {
      setSign(result, valueSign_1); // Итоговый знак совпадает со знаком первого числа
    }
    
  } else {
    // 3. ЕСЛИ ЗНАКИ ОДИНАКОВЫЕ: (+A) - (+B) или (-A) - (-B)
    // Требуется выравнивание масштабов
    alignmentScale(&value_1, &value_2);
    
    // Сохраняем выровненный scale (они теперь равны)
    int final_scale = getScale(value_1);
    
    // Создаем копии без знака для корректного сравнения мантисс
    s21_decimal abs_1 = value_1;
    s21_decimal abs_2 = value_2;
    setSign(&abs_1, 0);
    setSign(&abs_2, 0);
    
    if (s21_is_equal(abs_1, abs_2)) {
      // Если модули равны, результат всегда чистое золото — 0
      for (int x = 0; x < 4; x += 1) result->bits[x] = 0;
      setScale(result, final_scale);
    } else if (s21_is_less(abs_1, abs_2)) {
      // Если |A| < |B|, вычитаем из большего меньшее: |B| - |A|
      subBit(abs_2, abs_1, result);
      setScale(result, final_scale);
      setSign(result, !valueSign_1); // Знак инвертируется
    } else {
      // Если |A| > |B|, вычитаем: |A| - |B|
      subBit(abs_1, abs_2, result);
      setScale(result, final_scale);
      setSign(result, valueSign_1); // Знак остается как у первого
    }
  }
  
  return status;
}


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (!result) return 4;

  if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
	return 4;
  }
  
  // 1. Сброс результата и расчет знака
  for (int x = 0; x < 4; x++) result->bits[x] = 0;
  int sign_result = (getSign(value_1) != getSign(value_2));
  int scale_result = getScale(value_1) + getScale(value_2);
  
  // Очищаем знаки и масштабы у операндов для чистой работы с мантиссой
  setSign(&value_1, 0); setScale(&value_1, 0);
  setSign(&value_2, 0); setScale(&value_2, 0);

  s21_decimal tmp_result = {{0, 0, 0, 0}};
  int overflow = 0;

  // 2. Побитовое умножение в столбик
  int bit1 = getBitLast(value_1);
  for (int i = 0; i <= bit1; i++) {
    if (getBit(value_1, i)) {
      s21_decimal shifted_v2 = value_2;
      
      // Безопасный сдвиг: проверяем переполнение мантиссы перед/в процессе сдвига
      if (getBitLast(shifted_v2) + i > 95) {
        overflow = 1;
        break;
      }
      leftShift(&shifted_v2, i);
      
      // Складываем частичный результат
      if (addBit(tmp_result, shifted_v2, &tmp_result)) {
        overflow = 1;
        break;
      }
    }
  }

  // 3. Обработка переполнения за счет уменьшения масштаба (точности)
  // Если произошло переполнение мантиссы, но у нас есть запас по экспоненте
  while (overflow && scale_result > 0) {
    // Делаем шаг назад: возвращаем исходные числа, уменьшаем масштаб общего результата
    // Для этого восстанавливаем исходные структуры (упрощенный подход через уменьшение операндов)
    overflow = 0;
    if (getScale(value_1) > 0) decreaseScale(&value_1, 1);
    else if (getScale(value_2) > 0) decreaseScale(&value_2, 1);
    scale_result--;
    
    // Перезапуск умножения без рекурсии — просто очищаем и пересчитываем мантиссы
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

  // 4. Формирование финального статуса
  int status = 0;
  if (overflow || scale_result > 28) {
    status = sign_result ? 2 : 1; // 1 - слишком большое, 2 - слишком маленькое
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
  
  
  // 1. Очистка результата и базовые проверки
  for (int x = 0; x < 4; x++) result->bits[x] = 0;
  
  if (isNull(value_2)) return 3; // Ошибка: деление на 0
  if (isNull(value_1)) return 0; // 0 разделить на что угодно = 0

  int sign_result = (getSign(value_1) != getSign(value_2));
  int scale_1 = getScale(value_1);
  int scale_2 = getScale(value_2);
  
  // Приводим операнды к положительным числам с чистым масштабом
  setSign(&value_1, 0); setScale(&value_1, 0);
  setSign(&value_2, 0); setScale(&value_2, 0);

  s21_decimal quotient = {{0, 0, 0, 0}};
  s21_decimal remainder = {{0, 0, 0, 0}};
  s21_decimal temp_result = {{0, 0, 0, 0}};
  
  int final_scale = scale_1 - scale_2;
  int status = 0;

  // Алгоритм деления BigInt (мантисса на мантиссу) с фиксированной точкой
  for (int step = 0; step < 200; step++) { // Ограничитель шагов для дробной части
    
    // Основной цикл деления целой части текущего шага
    for (int i = 95; i >= 0; i--) {
      // Сдвигаем остаток влево на 1 и подтягиваем текущий бит делимого
      leftShift(&remainder, 1);
      setBit(&remainder, 0, getBit(value_1, i));
      
      // Если остаток стал больше или равен делителю
      if (s21_is_greater_or_equal(remainder, value_2)) {
        s21_sub(remainder, value_2, &remainder);
        leftShift(&quotient, 1);
        setBit(&quotient, 0, 1);
      } else {
        leftShift(&quotient, 1);
      }
    }

    // Прибавляем полученное частное к общему результату
    if (addBit(temp_result, quotient, &temp_result)) {
      status = sign_result ? 2 : 1;
      break;
    }

    // Если деление завершилось без остатка — выходим
    if (isNull(remainder)) {
      break;
    }

    // Если остаток есть, превращаем его в новое делимое (умножаем на 10)
    // и увеличиваем результирующий масштаб
    s21_decimal check_rem = remainder;
    increaseScale(&check_rem, 1);
    if (getScale(check_rem) == 0) { // Мантисса остатка переполнилась при x10
      break;
    }
    
    value_1 = remainder; // Новый делитель — это остаток * 10
    increaseScale(&value_1, 1);
    setScale(&value_1, 0);
    
    // Сдвигаем масштаб результата
    final_scale++;
    for (int x = 0; x < 4; x++) quotient.bits[x] = 0;
  }

  // 3. Выравнивание масштаба в допустимый диапазон [0..28]
  if (status == 0) {
    while (final_scale < 0) {
      increaseScale(&temp_result, 1);
      final_scale++;
    }
    while (final_scale > 28) {
      decreaseScale(&temp_result, 1);
      final_scale--;
    }
    
    // Дополнительная проверка на то, не превратилось ли число в 0 после сжатия масштаба
    if (isNull(temp_result) && !isNull(value_1)) {
      status = 2; // Число слишком маленькое для s21_decimal
    } else {
      *result = temp_result;
      setScale(result, final_scale);
      setSign(result, sign_result);
    }
  }

  return status;
}
