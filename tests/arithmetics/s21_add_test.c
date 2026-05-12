#include "../test_cases.h"

START_TEST(test_1) {
  s21_decimal value_1, value_2, result = {0};

  value_1.bits[0] = 0b11111111111111111111111111111111;
  value_1.bits[1] = 0b11111111111111111111111111111111;
  value_1.bits[2] = 0b11111111111111111111111111111111;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000001;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 1);
}
END_TEST

TCase* s21_add_test_case()
{
	TCase* tc;

	tc = tcase_create("s21_add");
	
	tcase_add_test(tc, test_1);

	return tc;
}
