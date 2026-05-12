#ifndef UTILS
#define UTILS

#include "s21_decimal.h"


///@brief Возвращает бит структуры десятичного числа
///@param x - ссылка на структуру десятичного числа
///@param y - номер бита
///@return значение бита
#define get_bit(x, y) (((x)->bits[(y) / 32] & (1u << ((y) % 32))) >> ((y) % 32))
///@brief Выставляет бит структуры десятичного числа
///@param x - ссылка на структуру десятичного числа
///@param y - номер бита
#define set_bit(x, y) ((x)->bits[(y) / 32] |= (1u << ((y) % 32)))
///@brief Снимает бит структуры десятичного числа
///@param x - ссылка на структуру десятичного числа
///@param y - номер бита
#define unset_bit(x, y) ((x)->bits[(y) / 32] &= ~(1u << ((y) % 32)))

///@brief Возвращает разименованую ссылку на нужный бит структуры десятичного
/// числа Так как возвращается ссылка на структуру, параметр можно не только
/// получить, но и задать
///@param x - ссылка на структуру десятичного числа
///@param y - какой параметр нужен sign -знак expo - экспонента
///@return разименованая ссылка на бит
#define get(x, y) ((s21_exposign*)x)->y

typedef struct s21_exposign {
  unsigned int bits[3];
  unsigned zero1 : 16;
  unsigned expo : 8;
  unsigned zero2 : 7;
  unsigned sign : 1;
} s21_exposign;

typedef struct binaryfloat {
  unsigned int mantisse : 23;
  unsigned int expo : 8;
  unsigned int sign : 1;
} binaryfloat;

/// @brief Проверяет корректность структуры десятичного числа
/// @param dec Decimal struct
/// @return 1 - корректно, 0 - не корректно
int is_dec_correct(s21_decimal dec);

/// @brief Заполняет структуру десятичного числа нулями
/// @param dec Decimal struct
void init_dec(s21_decimal* dec);

/// @brief Проверяет, является ли десятичное число нулевым
/// @param dec  Decimal struct
/// @return 1 - число является нулем, 0 - число не ноль
int isnull(s21_decimal dec);

/// @brief Приводит два числа к общей экспоненте.
/// @param dec1 Decimal struct
/// @param dec2 Decimal struct
/// @return Возвращает остаток от последнего сдвига экспоненты( от 0 до 9)
/// меньшего числа. Если dec1>dec2 остаток положительный, если dec2>dec1 остаток
/// отрицательный. Остаток 0, если удалось превести числа без потерь
int unidec(s21_decimal* dec1, s21_decimal* dec2);

/// @brief Складывает 2 мантиссы без учета знака и экспоненты
/// @param val1
/// @param val2
/// @param result
/// @return Возвращает 0 в случае успеха, 1 если результат слишком большой
int madd(s21_decimal val1, s21_decimal val2, s21_decimal* result);

/// @brief Складывает 2 подготовленных децимала с единой экспонентой без учета
/// знака
/// @param val1
/// @param val2
/// @return -1 если переполнение (экспонента меньше 0) иначе 0
int add_dev(s21_decimal val1, s21_decimal val2, s21_decimal* result, int* left);

/// @brief Вычитает мантиссу val2 из мантиссы val1 без учета знака и экспоненты
/// @param val1
/// @param val2
/// @param result
/// @return Возвращает 0 в случае успеха, 1 если результат слишком маленький
int msub(s21_decimal val1, s21_decimal val2, s21_decimal* result);

/// @brief Снижает экспоненту с учетом остатка от предыдущего снижения.
/// Позволяет сдвигать несколько децималов в ряд.
/// @param dec Децимал, который надо сдвинуть на 1 десятичный разряд вправо
/// @param left Остаток от сдвига стоящего слева децимала
/// @return Остаток от деления мантиссы на 10
int downexpo_extra(s21_decimal* dec, unsigned left);

/// @brief Умножает мантиссу на 10 и увеличивает экспоненту
/// @param dec Decimal struct
/// @return 0 если успех, 1 если экспонента вышла за ограничение в 28 или
/// закончились нули в начале числа
int raiseexpo(s21_decimal* dec);

/// @brief Умножает мантиссу на 10
/// @param dec  Decimal struct
/// @return 0 если успех, цифру вышедшую за левый край в результате умножения
/// или сдвига
unsigned int mul_by_ten(s21_decimal* dec);

/// @brief Integraly divide by 10 and down exponent
/// @param dec Decimal struct
/// @return -1 if error or last dropped digit
int downexpo(s21_decimal* dec);

/// @brief Находит значение бита по его номеру
/// @param val Decimal struct
/// @param bit Номер бита в числе
/// @return Возвращает значение хранящиеся в бите
// int get_bit(s21_decimal* val, int bit);

/// @brief Сравнивает мантиссы двух чисел.
/// @param dec1
/// @param dec2
/// @return Если числа равны 0, если первое больше 1, если второе больше -1
int mequal(s21_decimal dec1, s21_decimal dec2);

#ifdef DEBUG
/// @brief Печатает децимал в битовой форме
/// @param dec
void print_dec(s21_decimal dec);

#endif

/// @brief Округление по банковски (до четного)
/// @param dec
/// @param digit
/// @return 0 - Ok, 1 - Переполнение
int bank_round(s21_decimal* dec, int digit);

/// @brief Соединяет мантиссы двух децималов и корректирует экспоненту
/// @param dec
/// @param leftovers
/// @param left
/// @return 0 если все нормально, 1 если переполнение (экспонента уже 0, а в
/// lеftovers еще что-то есть)
int merge_dev(s21_decimal* dec, s21_decimal leftovers, int* left);

/// @brief Подготавливает децималы к делению
/// @param value_1
/// @param value_2
void prepare_div(s21_decimal* value_1, s21_decimal* value_2);

/// @brief Перемножает два децимала. Помещает результата в result  и
/// переполнение мантиссы в leftovers. Получа
/// @param value_1
/// @param value_2
/// @param result
/// @param leftovers
/// @return
int double_d_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result,
                 s21_decimal* leftovers);

/// @brief Переворачивает все биты мантиссы.(0xff -> 0x00, 0x11 -> 0xee). И
/// меняет знак
/// @param dec
void invert(s21_decimal* dec);


#endif
