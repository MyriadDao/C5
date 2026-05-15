#ifndef S21_DECIMAL_TYPE
#define S21_DECIMAL_TYPE

typedef struct
{
	int bits[4];
} s21_decimal;

typedef union {
  int ui;
  float fl;
} fbits;

#define S21_MAX_UINT 4294967295

#endif
