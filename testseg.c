#include <stdio.h>
#include <stdlib.h>
#include "seg.h"

static struct seg *seg;
static int out[1000];
static int outcnt;

static void visit(int node)
{
	int *set;
	int cnt;
	int i;
	if (!seg_nodeset(seg, node, &set, &cnt))
		for (i = 0; i < cnt; i++)
			out[outcnt++] = set[i];
}

static int intcmp(const void *a, const void *b)
{
	return *(int *) a - *(int *) b;
}

static int test_search(struct seg *seg, int x, int *exp, int n)
{
	int i;
	outcnt = 0;
	seg_search(seg, x, visit);
	if (outcnt != n)
		return 1;
	qsort(out, outcnt, sizeof(out[0]), intcmp);
	for (i = 0; i < n; i++)
		if (out[i] != exp[i])
			return 1;
	return 0;
}

static int test1(void)
{
	long beg[] = {0, 2, 3, 4, 5, -3};
	long end[] = {9, 9, 7, 6, 5, -2};
	int ret0[] = {0};
	int ret2[] = {0, 1};
	int ret5[] = {0, 1, 2, 3, 4};
	int ret8[] = {0, 1};
	int ret_2[] = {5};
	int ret_1[] = {5};
	seg = seg_init(6, beg, end);
	if (test_search(seg, 0, ret0, 1))
		return 1;
	if (test_search(seg, 2, ret2, 2))
		return 1;
	if (test_search(seg, 5, ret5, 5))
		return 1;
	if (test_search(seg, 8, ret8, 2))
		return 1;
	if (test_search(seg, -2, ret_2, 1))
		return 1;
	if (test_search(seg, -1, ret_1, 0))
		return 1;
	seg_free(seg);
	return 0;
}

static int test2(void)
{
	long beg[] = {2, 8, 9};
	long end[] = {5, 10, 9};
	int ret0[] = {};
	int ret3[] = {0};
	int ret9[] = {1, 2};
	seg = seg_init(3, beg, end);
	if (test_search(seg, 0, ret0, 0))
		return 1;
	if (test_search(seg, 3, ret3, 1))
		return 1;
	if (test_search(seg, 7, ret0, 0))
		return 1;
	if (test_search(seg, 9, ret9, 2))
		return 1;
	if (test_search(seg, 12, ret0, 0))
		return 1;
	seg_free(seg);
	return 0;
}

int main(void)
{
	if (test1())
		return 1;
	if (test2())
		return 2;
	return 0;
}
