#include <stdio.h>
#include <stdlib.h>
#include "seg.h"

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))

static int twodims = 0;		/* two dimensional trajectories */

/* a trajectory node */
struct node {
	long x, y, t;
};

/* a trajectory */
struct path {
	struct node *nodes;
	int n;
};

static int count_visits(struct path *path, struct seg *xseg, struct seg *yseg,
		long llx, long lly, long urx, long ury, long minT, long maxT)
{
	int visit_cnt = 0;
	void visit_node(int node)
	{
		int *set;
		int cnt;
		if (!seg_nodeset(xseg, node, &set, &cnt))
			visit_cnt += cnt;
	}
	seg_search(xseg, llx, visit_node);
	seg_search(xseg, urx, visit_node);
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
		tx2 = t1 + (urx - x1) * (t2 - t1) / (x2 - x1);
	if (x1 > x2 && x1 > urx)
		tx1 = t1 + (x1 - urx) * (t2 - t1) / (x1 - x2);
	if (x1 > x2 && x1 < urx && x2 < llx)
		tx1 = t1 + (x1 - llx) * (t2 - t1) / (x1 - x2);
	if (x1 > x2 && x2 < llx && x1 > urx)		/* double intersection */
		tx2 = t1 + (x1 - llx) * (t2 - t1) / (x1 - x2);
	if (tx1 > mintime)
		return tx1;
	if (tx2 > mintime)
		return tx2;
	return -1;
}

static int crossnorm(long ax, long ay, long bx, long by)
{
	long cross = (ax * by) - (ay * bx);
	if (!cross)
		return 0;
	return cross > 0 ? 1 : -1;
}

static long segmentsintersection(long ax, long ay, long bx, long by, long cx, long cy, long dx, long dy, long t1, long t2)
{
	if (crossnorm(bx - ax, by - ay, cx - ax, cy - ay) == crossnorm(bx - ax, by - ay, dx - ax, dy - ay))
		return -1;
	if (crossnorm(dx - cx, dy - cy, ax - cx, ay - cy) == crossnorm(dx - cx, dy - cy, bx - cx, by - cy))
		return -1;
	if (ax == bx && dx != cx)
		return t1 + (t2 - t1) * (ax - cx) / (dx - cx);
	if (ay == by && dy != cy)
		return t1 + (t2 - t1) * (ay - cy) / (dy - cy);
	return -1;
}

static long intersection2d(struct path *path, int edge, long mintime,
		long llx, long lly, long urx, long ury)
{
	long x1 = path->nodes[edge].x;
	long x2 = path->nodes[edge + 1].x;
	long y1 = path->nodes[edge].y;
	long y2 = path->nodes[edge + 1].y;
	long t1 = path->nodes[edge].t;
	long t2 = path->nodes[edge + 1].t;
	long txmin = -1;
	long tx[4];
	int i;
	tx[0] = segmentsintersection(llx, lly, urx, lly, x1, y1, x2, y2, t1, t2);
	tx[1] = segmentsintersection(llx, lly, llx, ury, x1, y1, x2, y2, t1, t2);
	tx[2] = segmentsintersection(llx, ury, urx, ury, x1, y1, x2, y2, t1, t2);
	tx[3] = segmentsintersection(urx, lly, urx, ury, x1, y1, x2, y2, t1, t2);
	for (i = 0; i < 4; i++)
		if (tx[i] > mintime && (txmin <= 0 || tx[i] < txmin))
			txmin = tx[i];
	return txmin;
}

static long first_visit(struct path *path, struct seg *xseg, struct seg *yseg, long ts,
		int qid, long llx, long lly, long urx, long ury)
{
	long best_ts = -1;
	struct seg *seg;
	void visit_node(int node)
	{
		int *set;
		int cnt;
		if (!seg_nodeset(seg, node, &set, &cnt)) {
			long dat = (long) seg_nodeget(seg, node);
			int dat_qid = dat >> 24;
			int idx = dat_qid == qid ? dat & 0xffffffl : 0;
			for (; idx < cnt; idx++) {
				long tx;
				if (twodims)
					tx = intersection2d(path, set[idx], ts, llx, lly, urx, ury);
				else
					tx = intersection(path, set[idx], ts, llx, lly, urx, ury);
				if (tx > 0 && (best_ts <= 0 || tx < best_ts))
					best_ts = tx;
				if (tx > 0)
					break;
			}
			dat = ((long) qid << 24) | (long) idx;
			seg_nodeput(seg, node, (void *) dat);
		}
	}
	seg = xseg;
	seg_search(xseg, llx, visit_node);
	seg_search(xseg, urx, visit_node);
	if (yseg) {
		seg = yseg;
		seg_search(yseg, lly, visit_node);
		seg_search(yseg, ury, visit_node);
	}
	return best_ts;
}

static int count_visits_duration(struct path *path, struct seg *xseg, struct seg *yseg,
		long llx, long lly, long urx, long ury, long minT, long maxT)
{
	long enter_ts;
	long leave_ts;
	long last_ts = 0;
	int cnt = 0;
	static int qid = 0;		/* query identifier */
	while (1) {
		if ((enter_ts = first_visit(path, xseg, yseg, last_ts,
						qid, llx, lly, urx, ury)) < 0)
			break;
		if ((leave_ts = first_visit(path, xseg, yseg, enter_ts,
						qid, llx, lly, urx, ury)) < 0)
			break;
		if (leave_ts - enter_ts >= minT && leave_ts - enter_ts <= maxT)
			cnt++;
		last_ts = leave_ts;
	}
	qid++;
	return cnt;
}

int main(int argc, char *argv[])
{
	int npaths, nqueries;
	struct path *paths;		/* input trajectories */
	struct seg **xsegs;		/* one segment tree for each trajectory */
	struct seg **ysegs;		/* one segment tree for each trajectory */
	int restrict_duration = 0;	/* restrict the duration of visits */
	int i, j;
	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] == 'd')
			restrict_duration = 1;
		if (argv[i][0] == '-' && argv[i][1] == '2')
			twodims = 1;
		if (argv[i][0] == '-' && argv[i][1] == 'h') {
			printf("Usage: %s options <input >output\n\n", argv[0]);
			printf("Options:\n");
			printf("  -d \t\t apply visit duration restrictions\n");
			printf("  -2 \t\t two-dimensional trajectories\n");
			return 0;
		}
	}
	if (twodims)
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
	xsegs = malloc(npaths * sizeof(xsegs[0]));
	ysegs = malloc(npaths * sizeof(ysegs[0]));
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
		xsegs[i] = seg_init(paths[i].n, beg, end);
		for (j = 0; j < paths[i].n - 1; j++) {
			long y1 = paths[i].nodes[j].y;
			long y2 = paths[i].nodes[j + 1].y;
			beg[j] = y1 < y2 ? y1 : y2;
			end[j] = y1 < y2 ? y2 : y1;
		}
		ysegs[i] = twodims ? seg_init(paths[i].n, beg, end) : NULL;
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
				visits += count_visits_duration(&paths[j], xsegs[j], ysegs[j],
					llx, lly, urx, ury, minT * 1000, maxT * 1000);
			} else {
				visits += count_visits(&paths[j], xsegs[j], ysegs[j],
					llx, lly, urx, ury, minT * 1000, maxT * 1000);
			}
		}
		printf("%d\n", visits);
	}

	/* release allocated memory */
	for (i = 0; i < npaths; i++)
		if (xsegs[i])
			seg_free(xsegs[i]);
	for (i = 0; i < npaths; i++)
		if (ysegs[i])
			seg_free(ysegs[i]);
	for (i = 0; i < npaths; i++)
		free(paths[i].nodes);
	free(paths);
	return 0;
}
