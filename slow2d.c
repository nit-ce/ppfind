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

static int answer(struct path *path, struct query *query);
static int sensitive_count_visits_x(struct node *bNode, struct node *eNode, struct query *query);
static int sensitive_count_visits_y(struct node *bNode, struct node *eNode, struct query *query);

int main()
{
	int numberOfPaths, numberOfQueries;
	struct path *paths;
	struct query *queries;
	int restrict_duration = 0;	/* restrict the duration of visits */
	int i, j;
	
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
	
	
	 /*answer queries*/
	 for (i = 0; i < numberOfQueries; i++) {
			int visits = 0;
			for (j = 0; j < numberOfPaths; j++)
				visits += answer(&paths[j], &queries[i]);
			printf("%d\n", visits);
		}
}

static int answer(struct path *path, struct query *query)
{
	int cnt = 0;
	int i;
	for (i = 0; i < path->n; i++) {
		if (path->nodes[i].y - path->nodes[i + 1].y == 0) {
			cnt += sensitive_count_visits_x(&path->nodes[i], &path->nodes[i + 1], query);
		}
		else if (path->nodes[i].x - path->nodes[i + 1].x == 0) {
			cnt += sensitive_count_visits_y(&path->nodes[i], &path->nodes[i + 1], query);
		}
	}
	
	return cnt;
}

static int sensitive_count_visits_x(struct node *bNode, struct node *eNode, struct query *query)
{
	int cnt = 0;
	
	long x1 = bNode->x;
	long x2 = eNode->x;
	long t1 = bNode->t;
	long t2 = eNode->t;
	long time = t2 - t1;
	long dist, outOfQry, inOfQry, insideTR, outsideTR;
	long rDiff, lDiff;
		 	
	if (x1 <= query->llx && x2 >= query->llx && x2 >= query->urx) {	
		if (x1 > x2) {
			long t = x1;
			x1 = x2;
			x2 = t;
		}
		dist = x2 - x1;
	
		
		lDiff = query->llx - x1;
 		rDiff = x2 - query->urx;				
			
		if (rDiff < 0) 
			rDiff = 0;
			
		outOfQry = lDiff + rDiff;
		inOfQry = dist - outOfQry;
		outsideTR = (outOfQry * time) / dist;
		insideTR = time - outsideTR;
	
		if (insideTR >= query->minT && insideTR <= query->maxT)
			cnt++;	
	}
	if (x1 > query->llx && x1 <= query->urx && x2 >= query->urx) {	
		if (x1 > x2) {
			long t = x1;
			x1 = x2;
			x2 = t;
		}
		dist = x2 - x1;
		
		lDiff = 0;
		rDiff = x2 - query->urx;
			
		outOfQry = lDiff + rDiff;
		inOfQry = dist - outOfQry;
		outsideTR = (outOfQry * time) / dist;
		insideTR = time - outsideTR;
		
		if (insideTR >= query->minT && insideTR <= query->maxT)
			cnt++;			
	}
	
	return cnt;
}

static int sensitive_count_visits_y(struct node *bNode, struct node *eNode, struct query *query)
{
	int cnt = 0;
	
	long y1 = bNode->y;
	long y2 = eNode->y;
	long t1 = bNode->t;
	long t2 = eNode->t;
	long time = t2 - t1;
	long dist, outOfQry, inOfQry, insideTR, outsideTR;
	long uDiff, lDiff;
		 
	if (y1 > y2) {
		long t = y1;
		y1 = y2;
		y2 = t;
	}
	dist = y2 - y1;
		
	if (y1 <= query->lly && y2 >= query->lly && y2 >= query->ury) {	
		lDiff = query->lly - y1;
 		uDiff = y2 - query->ury;				
			
//		if (uDiff < 0) 
//			uDiff = 0;
			
		outOfQry = lDiff + uDiff;
		inOfQry = dist - outOfQry;
		outsideTR = (outOfQry * time) / dist;
		insideTR = time - outsideTR;
	
		if (insideTR >= query->minT && insideTR <= query->maxT)
			cnt++;	
	}
	if (y1 > query->lly && y1 <= query->ury && y2 >= query->ury) {	
		lDiff = 0;
		uDiff = y2 - query->ury;
			
		outOfQry = lDiff + uDiff;
		inOfQry = dist - outOfQry;
		outsideTR = (outOfQry * time) / dist;
		insideTR = time - outsideTR;
		
		if (insideTR >= query->minT && insideTR <= query->maxT)
			cnt++;			
	}
	
	return cnt;
}
