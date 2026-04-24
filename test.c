#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int bits[4];
} s21_decimal;

// Индексы для удобства
#define LOW 0
#define MID 1
#define HIGH 2
#define INFO 3

// Коды ошибок
#define OK 0
#define TOO_LARGE 1
#define TOO_SMALL 2
#define DIV_BY_ZERO 3

// Работа с INFO (bits[3])
#define GET_SIGN(dec) ((dec.bits[INFO] >> 31) & 1)
#define GET_EXP(dec) ((dec.bits[INFO] >> 16) & 0xFF)
#define SET_SIGN(dec, sign) (dec->bits[INFO] = (dec->bits[INFO] & 0x7FFFFFFF) | (sign << 31))
#define SET_EXP(dec, exp) (dec->bits[INFO] = (dec->bits[INFO] & 0xFF00FFFF) | (exp << 16))

// Вспомогательная функция для сложения без учета знака и экспоненты
int base_add(s21_decimal v1, s21_decimal v2, s21_decimal *res) {
    uint64_t carry = 0;
    for (int i = 0; i < 3; i++) {
        uint64_t sum = (uint64_t)(uint32_t)v1.bits[i] + (uint32_t)v2.bits[i] + carry;
        res->bits[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    return carry ? TOO_LARGE : OK;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (!result) return OK;
    memset(result, 0, sizeof(s21_decimal));

    int sign1 = GET_SIGN(value_1);
    int sign2 = GET_SIGN(value_2);
    int exp1 = GET_EXP(value_1);
    int exp2 = GET_EXP(value_2);

    // Упрощение: работаем только с одинаковыми экспонентами и знаками (+)
    // В полной версии здесь нужна нормализация (приведение к общей экспоненте)
    if (sign1 == 0 && sign2 == 0) {
        if (exp1 == exp2) {
            int status = base_add(value_1, value_2, result);
            SET_EXP(result, exp1);
            if (status == TOO_LARGE) return TOO_LARGE;
        }
    } else if (sign1 != sign2) {
        // Здесь должен быть вызов s21_sub
    }

    return OK;
}
void print_decimal_raw(s21_decimal dec) {
    printf("Value: ");
    for (int i = 2; i >= 0; i--) {
        printf("%08X ", (uint32_t)dec.bits[i]); // Вывод мантиссы в HEX
    }
    printf("| Sign: %d | Exp: %d\n", (dec.bits[3] >> 31) & 1, (dec.bits[3] >> 16) & 0xFF);
}

#include <math.h>

void print_decimal(s21_decimal dec) {
    if ((dec.bits[3] >> 31) & 1) printf("-");

    // Склеиваем 96 бит в одно 128-битное число
    unsigned __int128 full_val = (uint32_t)dec.bits[2];
    full_val = (full_val << 32) | (uint32_t)dec.bits[1];
    full_val = (full_val << 32) | (uint32_t)dec.bits[0];

    int exp = (dec.bits[3] >> 16) & 0xFF;
    char buffer[40]; // 96 бит ~ 29 десятичных цифр
    int pos = 0;

    // Извлекаем цифры с конца
    if (full_val == 0) buffer[pos++] = '0';
    while (full_val > 0) {
        buffer[pos++] = (full_val % 10) + '0';
        full_val /= 10;
    }

    // Дополняем нулями, если экспонента больше количества цифр
    while (pos <= exp) buffer[pos++] = '0';

    // Печатаем в обратном порядке с точкой
    for (int i = pos - 1; i >= 0; i--) {
        if (i + 1 == exp) printf(".");
        printf("%c", buffer[i]);
    }
    printf("\n");
}


int main()
{

	//s21_decimal dec1 = {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
	s21_decimal dec1;
	memset(&dec1, 0, sizeof(dec1));
	dec1.bits[0] =  111;
	s21_decimal dec2 = {0};
	s21_decimal res;

	int result = s21_add(dec1, dec2, &res);

	printf("Decimal: ");
	print_decimal_raw(res);
	print_decimal(res);
	printf("Result: %d\n", result);
}
