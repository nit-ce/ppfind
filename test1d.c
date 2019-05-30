/* tests for one-dimensional trajectories */
#include <stdio.h>
#include <stdlib.h>

static void test00(void)
{
	printf("1\n");
	printf("4\n");
	printf("1 0 0\n");
	printf("2 10 0\n");
	printf("3 0 0\n");
	printf("4 5 0\n");
	printf("3\n");
	printf("1 0 2 0 0 10\n");
	printf("11 0 12 0 0 10\n");
	printf("6 0 8 0 0 10\n");
}

static void test05(void)
{
	int n = 1000;
	int q = 1000;
	int i;
	printf("1\n");
	printf("%d\n", n);
	for (i = 0; i < n; i++)
		printf("%d %d 0\n", i, (i % 2) ? 100 : 0);
	printf("%d\n", q);
	for (i = 0; i < q; i++)
		printf("%d 0 %d 0 0 1000\n", i % 200, (i % 200) + 10);
}

static void test06(void)
{
	int n = 1000;
	int q = 1000;
	int i;
	printf("1\n");
	printf("%d\n", n);
	for (i = 0; i < n; i++)
		printf("%d %d 0\n", i, i);
	printf("%d\n", q);
	for (i = 0; i < q; i++)
		printf("%d 0 %d 0 0 1000\n", i - 100, i - 40);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return 1;
	else if (atoi(argv[1]) == 0)
		test00();
	else if (atoi(argv[1]) == 5)
		test05();
	else if (atoi(argv[1]) == 6)
		test06();
	else
		return 1;
	return 0;
}
