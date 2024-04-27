#include <stdio.h>

int main()
{
	int main_scope_int = 42;

	void nested_func()
	{
		printf("NESTED FUNCTION %d\n", main_scope_int);
		return;
	}

	printf("MAIN FUNCTION\n");
	nested_func();

	return 0;
}
