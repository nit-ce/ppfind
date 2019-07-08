/* tests for one-dimensional trajectories */
#include <stdio.h>
#include <stdlib.h>

static void test00(void) 
{
	printf("1\n");
	printf("5\n");
	printf("1 1 1\n");
	printf("2 2 -1\n");
	printf("3 4 -1\n");
	printf("4 4 2\n");
	printf("5 5 0\n");
	
	printf("3\n");
	printf("0 -2 3 2 1 5");
	printf("0 -2 6 3 10 15");
	printf("5 -1 7 1 20 30");
}

static void test01(void) 
{
	printf("1\n");
	printf("5\n");
	printf("1 3 2\n");
	printf("2 5 2\n");
	printf("3 0 -3\n");
	printf("4 4 -1\n");
	printf("5 3 2\n");
	
	printf("3\n");
	printf("2 1 6 3 0 4");
	printf("2 0 3 1 0 10");
	printf("6 0 7 1 0 5");
}

static void test02(void) 
{
	printf("1\n");
	printf("8\n");
	printf("1 -1 2\n");
	printf("2 1 -2\n");
	printf("3 -1 2\n");
	printf("4 5 2\n");
	printf("5 6 4\n");
	printf("6 4 4\n");
	printf("7 6 -2\n");
	printf("8 7 -2\n");
	
	printf("4\n");
	printf("-2 0 0 3 0 10");
	printf("1 1 7 5 0 10");
	printf("1 -3 6 0 0 5");
}

static void test03(void) 
{
	printf("1\n");
	printf("4\n");
	printf("1 2 2\n");
	printf("2 4 2\n");
	printf("3 4 -2\n");
	printf("4 -2 -2\n");
	
	printf("1\n");
	printf("2 -2 4 2 0 10");
}

static void test04(void) 
{
	printf("1\n");
	printf("9\n");
	printf("1 0 1\n");
	printf("2 1 1\n");
	printf("3 1 0\n");
	printf("4 3 1\n");
	printf("5 4 2\n");
	printf("6 3 3\n");
	printf("7 2 4\n");
	printf("8 1 3\n");
	printf("9 0 3\n");
	
	printf("1\n");
	printf("1 1 3 2 0 10");
}

static void test05(void) 
{
	printf("1\n");
	printf("6\n");
	printf("1 0 2\n");
	printf("2 1 2\n");
	printf("3 0 1\n");
	printf("4 2 0\n");
	printf("5 2 2\n");
	printf("6 4 2\n");
	
	printf("1\n");
	printf("1 1 3 3 0 10");
}

static void test06(void) 
{
	printf("1\n");
	printf("10\n");
	printf("1 0 4\n");
	printf("2 3 3\n");
	printf("3 2 2\n");
	printf("4 1 3\n");
	printf("5 0 0\n");
	printf("6 4 0\n");
	printf("8 3 1\n");
	printf("9 2 2\n");
	printf("10 3 3\n");
	printf("11 3 10\n");
	
	printf("1\n");
	printf("1 1 3 3 0 10");
}

static void test07(void) 
{
	printf("1\n");
	printf("4\n");
	printf("1 0 0\n");
	printf("2 1 1\n");
	printf("3 2 1\n");
	printf("4 2 2\n");
	
	printf("1\n");
	printf("1 1 3 3 0 10");
}

int main(int argc, char *argv[])
{
	int num;
	if (argc != 2)
		return 1;
	num = atoi(argv[1]);
	if (num == 0)
		test00();
	else if (num == 1)
		test01();
	else if (num == 2)
		test02();
	else if (num == 3)
		test03();
	else if (num == 4)
		test04();
	else if (num == 5)
		test05();
	else if (num == 6)
		test06();
	else if (num == 7)
		test07();
	
	return 0;
}
