#include <cstdio>
#include <unistd.h>

/**
 * This program was used to test the shell (jobs part).
 */
int main()
{
	for (int i = 0; i < 5; ++i)
	{
		printf("A\n");
		sleep(1);
	}
	return 0;
}
