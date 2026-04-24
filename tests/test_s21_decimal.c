#include "decimal_suite.h"

int main()
{
	Suite* suite = decimal_suite();
	SRunner* runner = srunner_create(suite);

	srunner_run_all(runner, CK_NORMAL);

	srunner_free(runner);
	return 0;
}
