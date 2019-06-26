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

//path will finish at the start of the second query
void test01()
{
	printf("1\n");
	printf("5\n");
	printf("1 0 0\n");
	printf("2 3 0\n");
	printf("3 2 0\n");
	printf("4 5 0\n");
	printf("5 10 0\n");
	printf("3\n");
	printf("1 0 2.5 0 0 10\n");//asn: 3
	printf("10 0 11 0 0 10\n");//ans: 0
	printf("1 0 4 0 0 20\n");//ans: 3
}

//all path (instead one of them) will be in the last query
void test02()
{
	printf("1\n");
	printf("5\n");
	printf("1 1 0\n");
	printf("2 2 0\n");
	printf("3 3 0\n");
	printf("4 6 0\n");
	printf("5 1 0\n");
	printf("2\n");
	printf("-1 0 0 0 0 10\n");//ans: 0
	printf("1 0 4 0 0 10\n");//ans: 4
}
//path starts ,at start of one query(q1) | finishes in the middle of another query(q2)
void test03()
{
	printf("1\n");
	printf("5\n");
	printf("1 2 0\n");
	printf("2 5 0\n");
	printf("3 7 0\n");
	printf("4 2.8 0\n");
	printf("5 8.8 0\n");
	printf("2\n");
	printf("2 0 3 0 0 10\n");//ans: 4
	printf("8 0 9 0 0 15\n");// ans: 1
}

//start and end of the path is the same in query(q1 & q2)
void test04()
{
	printf("1\n");
	printf("7\n");
	printf("1 1 0\n");
	printf("2 2 0\n");
	printf("3 3 0\n");
	printf("4 4 0\n");
	printf("5 5 0\n");
	printf("6 3.5 0\n");
	printf("7 1 0\n");
	printf("3\n");
	printf("0 0 1 0 0 10\n");//ans: 1
	printf("1 0 2.5 0 0 10\n");// ans: 2
	printf("2.5 0 4.5 0 0 10\n");// ans: 5
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
	for (int i = 0; i < n; i++)
	{
		printf("%d %d 0\n", i, c + i);
		if ((c + i) % 5 == 0)
		{
			c += 10;
		}
	}

	printf("%d\n", q);
	for (int i = 0; i < q; i++)
	{
		printf("%d 0 %d 0 0 %d\n", i + l, i + r, n);
	}
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
	for (int i = 0; i < n; i++)
	{
		int c = rand() % (maxRange + 1 - minRange) + minRange;
		s += i * c;
		printf("%d %d 0\n", i, s);
		if (s % 3 == 0)
			s -= 20;
	}

	int l = 15;
	int r = 20;
	printf("%d\n", q);
	for (int i = 0; i < q; i++)
	{
		printf("%d 0 %d 0 0 %d\n", l, r, n);
		if (i % 10 == 0)
		{
			l -= 15;
			r -= 15;
		}
		else
		{
			l += 10;
			r += 10;
		}
	}
}

static void test09()
{
	printf("100\n");
 	for (int i = 0; i< 100 ;i++)
	{
   		printf("99\n");
    	for(int j = 0; j<99 ; j++)
    	{
          printf("%d 0 0\n", i);
          printf("%d 0 1\n", i+1);
          printf("%d 0 0\n" ,i+2);
          j= j+2;
        }
	}

  	printf("1\n");
 	printf("0 0 1 0 0 100\n");
}

//tests by considering time limits
//a test with two way
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
  	printf("4\n");
  	printf("1 4 0\n");
  	printf("5 -7 0\n");
  	printf("7 11 0\n");

  	printf("2\n");
  	printf("0 0 2 0 4 6\n");
  	printf("4 0 5 0 2 7\n");
}

//in middle of time and at the edge of the path
static void test12()
{
	printf("1\n");
  	printf("5\n");
  	printf("0 0 0\n");
  	printf("12 10 0\n");
  	printf("13 6 0\n");
  	printf("14 5 0\n");
  	printf("15 0 0\n");

  	printf("3\n");
  	printf("0 0 10 0 0 14\n");//ans: 1
  	printf("5 0 6 0 13 15\n");//ans: 1
  	printf("1 0 2 0 4 14\n");//ans: 0
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
