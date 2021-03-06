#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))

static int twodims = 1;		/* two dimensional trajectories */

/* a trajectory node */
struct node {
	long x, y, t;
};

/* a trajectory */
struct path {
	struct node *nodes;
	int n;
};

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

static long first_visit(struct path *path, long ts,
		int qid, long llx, long lly, long urx, long ury)
{
	static struct path *last_path = NULL;
	static int last_qid = -1;
	static int idx = 0;
	if (last_path != path || last_qid != qid)
		idx = 0;
	last_path = path;
	last_qid = qid;
	for (; idx < path->n; idx++) {
		long tx;
		if (twodims)
			tx = intersection2d(path, idx, ts, llx, lly, urx, ury);
		else
			tx = intersection(path, idx, ts, llx, lly, urx, ury);
		if (tx > 0)
			return tx;
	}
	return -1;
}

static int count_visits(struct path *path, long llx, long lly, long urx,
		long ury, long minT, long maxT)
{
	long enter_ts;
	long leave_ts;
	long last_ts = 0;
	int cnt = 0;
	static int qid = 0;		/* query identifier */
	while (1) {
		if ((enter_ts = first_visit(path, last_ts,
						qid, llx, lly, urx, ury)) < 0)
			break;
		if ((leave_ts = first_visit(path, enter_ts,
						qid, llx, lly, urx, ury)) < 0)
			break;
		if ((minT < 0 || leave_ts - enter_ts >= minT) &&
				(maxT < 0 || leave_ts - enter_ts <= maxT))
			cnt++;
		last_ts = leave_ts;
	}
	qid++;
	return cnt;
}

/* current time stamp in milliseconds */
static long timestamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main(int argc, char *argv[])
{
	int npaths, nqueries;
	struct path *paths;		/* input trajectories */
	int restrict_duration = 1;	/* restrict the duration of visits */
	int i, j;
	long ts0, ts1, ts2, ts3;
	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] == 'n')
			restrict_duration = 0;
		if (argv[i][0] == '-' && argv[i][1] == '1')
			twodims = 0;
		if (argv[i][0] == '-' && argv[i][1] == 'h') {
			printf("Usage: %s options <input >output\n\n", argv[0]);
			printf("Options:\n");
			printf("  -n \t\t no visit duration restrictions\n");
			printf("  -1 \t\t one-dimensional trajectories\n");
			return 0;
		}
	}
	if (twodims)
		restrict_duration = 1;
	ts0 = timestamp();
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
	ts1 = timestamp();
	ts2 = timestamp();
	/* answer input queries */
	scanf("%d", &nqueries);
	for (i = 0; i < nqueries; i++) {
		int visits = 0;
		long llx, lly, urx, ury;
		long minT, maxT;
		scanf("%ld %ld %ld %ld", &llx, &lly, &urx, &ury);
		scanf("%ld %ld", &minT, &maxT);
		for (j = 0; j < npaths; j++) {
			visits += count_visits(&paths[j], llx, lly, urx, ury,
				restrict_duration ? minT * 1000 : -1,
				restrict_duration ? maxT * 1000 : -1);
		}
		printf("%d\n", visits);
	}
	ts3 = timestamp();
	fprintf(stderr, "%ld %ld %ld\n", ts1 - ts0, ts2 - ts1, ts3 - ts2);
	/* release allocated memory */
	for (i = 0; i < npaths; i++)
		free(paths[i].nodes);
	free(paths);
	return 0;
}
