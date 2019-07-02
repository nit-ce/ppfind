#include <stdio.h>
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

static int test_search(struct seg *seg, int x, int *exp, int n)
{
	int i;
	outcnt = 0;
	seg_search(seg, x, visit);
	if (outcnt != n)
		return 1;
	for (i = 0; i < n; i++)
		if (out[i] != exp[i])
			return 1;
	return 0;
}

static int test0(void)
{
	long beg[] = {2, 8, 9};
	long end[] = {5, 10, 9};
	int ret3[] = {0};
	int ret9[] = {1, 2};
	seg = seg_init(3, beg, end);
	if (test_search(seg, 3, ret3, 1))
		return 1;
	if (test_search(seg, 9, ret9, 2))
		return 1;
	seg_free(seg);
	return 0;
}

int main(void)
{
	if (test0())
		return 1;
	return 0;
}
