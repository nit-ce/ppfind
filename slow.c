#include <stdio.h>
#include <stdlib.h>

/* a trajectory node */
struct node {
	long x, y, t;
};

/* a trajectory */
struct path {
	struct node *nodes;
	int n;
};

/* a query */
struct query {
	long llx, lly;		/* lower left corner */
	long urx, ury;		/* upper right corner */
	long minT, maxT;	/* minimum and maximum visit duration */
};

static int count_visits(struct path *path, struct query *query);
static int sensitive_count_visits(struct path *path, struct query *query);
static long timeRatio(long lDiff, long rDiff, long dist, long time);

int main(int argc, char *argv[])
{
	int numberOfPaths, numberOfQueries;
	struct path *paths;
	struct query *queries;
	int restrict_duration = 0;	/* restrict the duration of visits */
	int i, j;

	if (argc > 1 && atoi(argv[1]) > 0)
		restrict_duration = 1;

	/* read input trajectories */
	scanf("%d", &numberOfPaths);
	paths = malloc(numberOfPaths * sizeof(paths[0]));
	for (i = 0; i < numberOfPaths; i++) {
		scanf("%d", &paths[i].n);
		paths[i].nodes = malloc(paths[i].n * sizeof(paths[i].nodes[0]));
		for (j = 0; j < paths[i].n; j++) {
			scanf("%ld", &paths[i].nodes[j].t);
			scanf("%ld", &paths[i].nodes[j].x);
			scanf("%ld", &paths[i].nodes[j].y);
		}
	}

	/* read input queries */
	scanf("%d", &numberOfQueries);
	queries = malloc(numberOfQueries * sizeof(queries[0]));
	for (i = 0; i < numberOfQueries; i++) {
		scanf("%ld", &queries[i].llx);
		scanf("%ld", &queries[i].lly);
		scanf("%ld", &queries[i].urx);
		scanf("%ld", &queries[i].ury);
		scanf("%ld", &queries[i].minT);
		scanf("%ld", &queries[i].maxT);
	}

	/* answer the queries */
	if (restrict_duration == 0) {
		for (i = 0; i < numberOfQueries; i++) {
			int visits = 0;
			for (j = 0; j < numberOfPaths; j++)
				visits += count_visits(&paths[j], &queries[i]);
			printf("%d\n", visits);
		}		
	} else {
		for (i = 0; i < numberOfQueries; i++) {
			int visits = 0;
			for (j = 0; j < numberOfPaths; j++)
				visits += sensitive_count_visits(&paths[j], &queries[i]);
			printf("%d\n", visits);
		}
	}

	/* release allocated memory */
	for (i = 0; i < numberOfPaths; i++)
		free(paths[i].nodes);
	free(paths);
	free(queries);
	return 0;
}

static int count_visits(struct path *path, struct query *query)
{
	int cnt = 0;		/* visit count */
	int i;
	for (i = 0; i < path->n - 1; i++) {
		long x1 = path->nodes[i].x;
		long x2 = path->nodes[i + 1].x;
		if (x1 > x2) {
			long t = x1;
			x1 = x2;
			x2 = t;
		}
		if (x1 <= query->llx && x2 >= query->llx)
			cnt++;
		if (x1 > query->llx && x1 <= query->urx && x2 >= query->urx)
			cnt++;
	}
	return cnt;
}
//find answers according to the timw
static int sensitive_count_visits(struct path *path, struct query *query)
{
	int cnt = 0;		/* visit count */
	int i;
	for (i = 0; i < path->n - 1; i++) {
		long x1 = path->nodes[i].x;
		long x2 = path->nodes[i + 1].x;
		long t1 = path->nodes[i].t;		/*start time*/
		long t2 = path->nodes[i + 1].t;		/*finsish time*/
		long time = t2 - t1;	/*difference between start and finsih times*/
		long dist rDiff, lDiff;		/*lDiff: store distance difference between query.llx and path.node[i].x | rDiff: store distance difference between query.urx and path.node[i+1].x*/
		 
		if (x1 > x2) {
			long t = x1;
			x1 = x2;
			x2 = t;
		}
		dist = x2 - x1;
		
		if (x1 <= query->llx && x2 >= query->llx) {
			
			lDiff = query->llx - x1;
 			rDiff = x2 - query->urx;				
			
			if (rDiff < 0) 
				rDiff = 0;
			
			long insideTR = timeRatio(lDiff, rDiff, dist, time);	/*store time ratio: howmuch time it is inside the query zone*/
	
			if (insideTR >= query->minT && insideTR <= query->maxT)
				cnt++;	
		}
		if (x1 > query->llx && x1 <= query->urx && x2 >= query->urx) {
			
			lDiff = 0;
			rDiff = x2 - query->urx;
			
			long insideTR = timeRatio(lDiff, rDiff, dist, time);
			
			if (insideTR >= query->minT && insideTR <= query->maxT)
				cnt++;			
		}
	}
	return cnt;
}

/*calculate howmuch time it is outside query zone and return howmuch time was inside the query zone*/
static long timeRatio(long lDiff, long rDiff, long dist, long time)
{
	long outOfQry, inOfQry, insideTR, outsideTR;
	
	outOfQry = lDiff + rDiff;
	inOfQry = dist - outOfQry;
	outsideTR = (outOfQry * time) / dist;
	insideTR = time - outsideTR;
	
	return insideTR;
}
