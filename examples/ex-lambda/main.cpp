#include <stdio.h>

int main()
{
	auto nested_func = []() {
		printf("NESTED FUNCTION\n");
	};

	nested_func();
	printf("MAIN FUNCTION\n");
	return 0;
}
