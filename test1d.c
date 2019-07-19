/* tests for one-dimensional trajectories */
#include <stdio.h>
#include <stdlib.h>

/* problem description test case */
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

/* path will finish at the start of the second query */
void test01()
{
	printf("1\n");
	printf("5\n");
	printf("1 0 0\n");
	printf("2 30 0\n");
	printf("3 20 0\n");
	printf("4 50 0\n");
	printf("5 90 0\n");
	printf("3\n");
	printf("21 0 29 0 0 10\n");
	printf("91 0 95 0 0 10\n");
	printf("10 0 40 0 0 10\n");
}

//all path (instead of one of them) will be in the last query
void test02()
{
	printf("1\n");
	printf("5\n");
	printf("1 10 0\n");
	printf("2 20 0\n");
	printf("3 30 0\n");
	printf("4 60 0\n");
	printf("5 10 0\n");
	printf("2\n");
	printf("-1 0 0 0 0 10\n");
	printf("15 0 40 0 0 10\n");
}

void test03()
{
	printf("1\n");
	printf("5\n");
	printf("1 0 0\n");
	printf("2 5 0\n");
	printf("3 10 0\n");
	printf("4 5 0\n");
	printf("5 0 0\n");
	printf("4\n");
	printf("3 0 7 0 0 10\n");
	printf("2 0 4 0 0 10\n");
	printf("6 0 8 0 0 10\n");
	printf("6 0 12 0 0 10\n");
}

/* start and end of the path is the same in query(q1 & q2) */
void test04()
{
	printf("1\n");
	printf("7\n");
	printf("1 10 0\n");
	printf("2 20 0\n");
	printf("3 30 0\n");
	printf("4 40 0\n");
	printf("5 50 0\n");
	printf("6 35 0\n");
	printf("7 10 0\n");
	printf("3\n");
	printf("11 0 15 0 0 10\n");
	printf("12 0 25 0 0 10\n");
	printf("25 0 45 0 0 10\n");
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

//time checking
void test07()
{
	int n = 1000;
	int q = 1000;
	int c = 10;
	int l = 2;
	int r = 5;

	printf("1\n");
	printf("%d\n", n);
	for (int i = 0; i < n; i++) {
		printf("%d %d 0\n", i, c + i);
		if ((c + i) % 5 == 0)
			c += 10;
	}

	printf("%d\n", q);
	for (int i = 0; i < q; i++)
		printf("%d 0 %d 0 0 %d\n", i + l, i + r, n);
}

//time checking
void test08()
{
	int n = 1000;
	int q = n + 500;
	int maxRange = 4;
	int minRange = 2;
	int s = 0;

	printf("1\n");
	printf("%d\n", n);
	for (int i = 0; i < n; i++) {
		int c = rand() % (maxRange + 1 - minRange) + minRange;
		s += i * c;
		printf("%d %d 0\n", i, s);
		if (s % 3 == 0)
			s -= 20;
	}

	int l = 15;
	int r = 20;
	printf("%d\n", q);
	for (int i = 0; i < q; i++) {
		printf("%d 0 %d 0 0 %d\n", l, r, n);
		if (i % 10 == 0) {
			l -= 15;
			r -= 15;
		} else {
			l += 10;
			r += 10;
		}
	}
}

static void test09()
{
	printf("100\n");
	for (int i = 0; i < 100; i++) {
		printf("99\n");
		for(int j = 0; j < 99; j++) {
			printf("%d 0 0\n", i);
			printf("%d 0 1\n", i+1);
			printf("%d 0 0\n" ,i+2);
			j= j+2;
		}
	}
	printf("1\n");
	printf("0 0 1 0 0 100\n");
}

/* tests with restrictions on visit duration */
/* testing two trajectories */
static void test10()
{
	printf("2\n");

	printf("4\n");
	printf("0 0 0\n");
	printf("5 4 0\n");
	printf("15 5 0\n");
	printf("25 -3 0\n");

	printf("3\n");
	printf("5 -10 0\n");
	printf("15 10 0\n");
	printf("25 -10 0\n");

	printf("4\n");
	printf("0 0 4 0 0 10\n");
	printf("4 0 5 0 0 22\n");
	printf("-3 0 0 0 24 25\n");
	printf("-10 0 9 0 0 25\n");
}

//in middle of time and in middle of the path
static void test11()
{
	printf("1\n");
	printf("3\n");
	printf("0 0 0\n");
	printf("10 10 0\n");
	printf("20 20 0\n");

	printf("8\n");
	printf("5 0 6 0 2 10\n");
	printf("5 0 7 0 2 10\n");
	printf("5 0 8 0 2 10\n");
	printf("5 0 15 0 2 10\n");
	printf("5 0 6 0 0 2\n");
	printf("5 0 7 0 0 2\n");
	printf("5 0 8 0 0 2\n");
	printf("5 0 15 0 0 2\n");
}

//in middle of time and at the edge of the path
static void test12()
{
	printf("1\n");
	printf("5\n");
	printf("0 0 0\n");
	printf("12 100 0\n");
	printf("13 60 0\n");
	printf("14 50 0\n");
	printf("15 0 0\n");

	printf("3\n");
	printf("1 0 99 0 0 14\n");//ans: 1
	printf("49 61 6 0 13 15\n");//ans: 1
	printf("10 0 20 0 4 14\n");//ans: 0
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
	else if (num == 8)
		test08();
	else if (num == 9)
		test09();
	else if (num == 10)
		test10();
	else if (num == 11)
		test11();
	else if (num == 12)
		test12();
	return 0;
}
