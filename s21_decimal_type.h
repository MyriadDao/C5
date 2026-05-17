#ifndef S21_DECIMAL_TYPE
#define S21_DECIMAL_TYPE

#include <stdint.h>

typedef struct
{
	int bits[4];
} s21_decimal;

typedef union {
  int ui;
  float fl;
} fbits;

typedef union decimal_bit3 {
    int i;
    struct {
        uint32_t empty2 : 16;
        uint32_t power : 8;
        uint32_t empty1 : 7;
        uint32_t sign : 1;
    } parts;
} decimal_bit3;

#define S21_MAX_UINT 4294967295

#endif
