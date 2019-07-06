#include <stdio.h>
#include <stdlib.h>
#include "seg.h"

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))

/* a trajectory node */
struct node {
	long x, y, t;
};

/* a trajectory */
struct path {
	struct node *nodes;
	int n;
};

static int count_visits(struct path *path, struct seg *seg,
		long llx, long lly, long urx, long ury, long minT, long maxT)
{
	int visit_cnt = 0;
	void visit_node(int node)
	{
		int *set;
		int cnt;
		if (!seg_nodeset(seg, node, &set, &cnt))
			visit_cnt += cnt;
	}
	seg_search(seg, llx, visit_node);
	seg_search(seg, urx, visit_node);
	return visit_cnt >> 1;
}

static long intersection(struct path *path, int edge, long mintime,
		long llx, long lly, long urx, long ury)
{
	long x1 = path->nodes[edge].x;
	long x2 = path->nodes[edge + 1].x;
	long t1 = path->nodes[edge].t;
	long t2 = path->nodes[edge + 1].t;
	long tx1 = -1;		/* the first intersection */
	long tx2 = -1;		/* the second intersection */
	if (t2 <= mintime)
		return -1;
	if (MIN(x1, x2) > urx || MAX(x1, x2) < llx)	/* they cannot intersect */
		return -1;
	if (x1 < x2 && x1 < llx)
		tx1 = t1 + (llx - x1) * (t2 - t1) / (x2 - x1);
	if (x1 < x2 && x1 > llx && x2 > urx)
		tx1 = t1 + (urx - x1) * (t2 - t1) / (x2 - x1);
	if (x1 < x2 && x1 < llx && x2 > urx)		/* double intersection */
		tx2 = t1 + (ury - x1) * (t2 - t1) / (x2 - x1);
	if (x1 > x2 && x1 > urx)
		tx1 = t1 + (x1 - urx) * (t2 - t1) / (x1 - x2);
	if (x1 > x2 && x1 < urx && x2 < llx)
		tx1 = t1 + (x1 - llx) * (t2 - t1) / (x1 - x2);
	if (x1 > x2 && x2 > urx && x2 < llx)		/* double intersection */
		tx2 = t1 + (x1 - llx) * (t2 - t1) / (x1 - x2);
	if (tx1 > mintime)
		return tx1;
	if (tx2 > mintime)
		return tx2;
	return -1;
}

static long first_visit(struct path *path, struct seg *seg, long ts,
		long llx, long lly, long urx, long ury)
{
	long best_ts = -1;
	void visit_node(int node)
	{
		int *set;
		int cnt;
		if (!seg_nodeset(seg, node, &set, &cnt)) {
			long idx = (long) seg_nodeget(seg, node);
			for (; idx < cnt; idx++) {
				long tx = intersection(path, set[idx], ts, llx, lly, urx, ury);
				if (tx > 0 && (best_ts <= 0 || tx < best_ts))
					best_ts = tx;
				if (tx)
					break;
			}
			seg_nodeput(seg, node, (void *) idx);
		}
	}
	seg_search(seg, llx, visit_node);
	seg_search(seg, urx, visit_node);
	return best_ts;
}

static int count_visits_duration(struct path *path, struct seg *seg,
		long llx, long lly, long urx, long ury, long minT, long maxT)
{
	long enter_ts;
	long leave_ts;
	long last_ts = 0;
	int cnt = 0;
	while (1) {
		if ((enter_ts = first_visit(path, seg, last_ts, llx, lly, urx, ury)) < 0)
			break;
		if ((leave_ts = first_visit(path, seg, enter_ts, llx, lly, urx, ury)) < 0)
			break;
		if (leave_ts - enter_ts >= minT && leave_ts - enter_ts <= maxT)
			cnt++;
		last_ts = leave_ts;
	}
	return cnt;
}

int main(int argc, char *argv[])
{
	int npaths, nqueries;
	struct path *paths;		/* input trajectories */
	struct seg **segs;		/* one segment tree for each trajectory */
	int restrict_duration = 0;	/* restrict the duration of visits */
	int i, j;
	if (argc > 1 && atoi(argv[1]) > 0)
		restrict_duration = 1;

	/* read input trajectories */
	scanf("%d", &npaths);
	paths = malloc(npaths * sizeof(paths[0]));
	for (i = 0; i < npaths; i++) {
		scanf("%d", &paths[i].n);
		paths[i].nodes = malloc(paths[i].n * sizeof(paths[i].nodes[0]));
		for (j = 0; j < paths[i].n; j++) {
			scanf("%ld", &paths[i].nodes[j].t);
			scanf("%ld", &paths[i].nodes[j].x);
			scanf("%ld", &paths[i].nodes[j].y);
			paths[i].nodes[j].t *= 1000;
		}
	}

	/* construct the segment trees */
	segs = malloc(npaths * sizeof(segs[0]));
	for (i = 0; i < npaths; i++) {
		long *beg, *end;
		beg = malloc(paths[i].n * sizeof(beg[0]));
		end = malloc(paths[i].n * sizeof(end[0]));
		for (j = 0; j < paths[i].n - 1; j++) {
			long x1 = paths[i].nodes[j].x;
			long x2 = paths[i].nodes[j + 1].x;
			beg[j] = x1 < x2 ? x1 : x2;
			end[j] = x1 < x2 ? x2 : x1;
		}
		segs[i] = seg_init(paths[i].n, beg, end);
		free(beg);
		free(end);
	}

	/* answer input queries */
	scanf("%d", &nqueries);
	for (i = 0; i < nqueries; i++) {
		int visits = 0;
		for (j = 0; j < npaths; j++) {
			long llx, lly, urx, ury;
			long minT, maxT;
			scanf("%ld %ld %ld %ld", &llx, &lly, &urx, &ury);
			scanf("%ld %ld", &minT, &maxT);
			if (restrict_duration) {
				visits += count_visits_duration(&paths[j], segs[j],
					llx, lly, urx, ury, minT * 1000, maxT * 1000);
			} else {
				visits += count_visits(&paths[j], segs[j],
					llx, lly, urx, ury, minT * 1000, maxT * 1000);
			}
		}
		printf("%d\n", visits);
	}

	/* release allocated memory */
	for (i = 0; i < npaths; i++)
		seg_free(segs[i]);
	for (i = 0; i < npaths; i++)
		free(paths[i].nodes);
	free(paths);
	return 0;
}
