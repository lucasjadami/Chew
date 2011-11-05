#include <cstdio>
#include <unistd.h>

int main()
{
	for (int i = 0; i < 5; ++i)
	{
		printf("A\n");
		sleep(1);
	}
	return 0;
}
