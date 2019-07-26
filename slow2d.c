#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
static int sensitive_count_visits_x_y(struct node *bNode, struct node *eNode, struct query *query);
static int findCrossingPoint(long x1, long x2, long y1, long y2, long t1, long t2, struct query *query);
static int isInside(long x1, long y1, long x2, long y2, long xA, long yA, long t1, long t2, long minT, long maxT);
static int isInside_2(long x1, long y1, long x2, long y2, long xA, long yA, long xA_2, long yA_2, long t1, long t2, long minT, long maxT, int flag);

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
		printf("v: %d\n", visits);
	}	
}

static int answer(struct path *path, struct query *query)
{
	int cnt = 0;
	int i;
	for (i = 0; i < path->n; i++) {
		if (path->nodes[i].y - path->nodes[i + 1].y == 0) {
			cnt += sensitive_count_visits_x(&path->nodes[i], &path->nodes[i + 1], query);
		} else if (path->nodes[i].x - path->nodes[i + 1].x == 0) {
			cnt += sensitive_count_visits_y(&path->nodes[i], &path->nodes[i + 1], query);
		} else {
			cnt += sensitive_count_visits_x_y(&path->nodes[i], &path->nodes[i + 1], query);
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
		 	
	if ((x1 <= query->llx && x2 >= query->urx) || (x1 >= query->urx && x2 < query->llx)) {	
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
	if (x1 > query->llx && x1 <= query->urx && (x2 > query->urx || x2 < query->llx)) {	
		
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
		
	if ((y1 <= query->lly &&  y2 > query->ury) || (y1 >= query->ury && y2 < query->lly)) {	
		if (y1 > y2) {
			long t = y1;
			y1 = y2;
			y2 = t;
		}
		dist = y2 - y1;
		
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
	if (y1 > query->lly && y1 <= query->ury && (y2 > query->ury || y2 < query->llx)) {	
		if (y1 > y2) {
			long t = y1;
			y1 = y2;
			y2 = t;
		}
		dist = y2 - y1;
		
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

static int sensitive_count_visits_x_y(struct node *bNode, struct node *eNode, struct query *query) {
	int cnt = 0;
	long x1 = bNode->x;
	long x2 = eNode->x;
	long y1 = bNode->y;
	long y2 = eNode->y;
	long t1 = bNode->t;
	long t2 = eNode->t;
	long time = t2 - t1;
	
	if (x2 > query->llx && x2 < query->urx && y2 > query->lly && y2 < query->ury) {
		cnt = 0;
	} else {	
		if (y1 > y2) {
			long t = y1;
			y1 = y2;
			y2 = t1;
		}
		if (x1 >= query->llx && x1 <= query->urx && y1 >= query->lly && y1 <= query->ury) {
			 if (x2 > query->urx && y2 >= query->lly && y2 <= query->ury) {
			 	cnt += findCrossingPoint(x1, x2, y1, y2, t1, t2, query);
			 } else if (y2 > query->ury &&  x2 >= query->llx && x2 <= query->urx) {
			 	cnt += findCrossingPoint(x1, x2, y1, y2, t1, t2, query);
			 } else if (x2 > query->urx && y2 > query->ury ) {
			 	cnt += findCrossingPoint(x1, x2, y1, y2, t1, t2, query);
			 } else if (x2 < query->llx && y2 > query->ury) {
			 	cnt += findCrossingPoint(x1, x2, y1, y2, t1, t2, query);
			 }
		} else {
			cnt += findCrossingPoint(x1, x2, y1, y2, t1, t2, query);
		}
	}
	
	return cnt;
}

static int findCrossingPoint(long x1, long x2, long y1, long y2, long t1, long t2, struct query *query) {
	
	int cnt = 0;
	long x3 = query->llx;
	long x4 = query->urx;
	long y3 = query->lly;
	long y4 = query->ury;
	long xA = 0;
	long yA = 0;
	double t = 0;
	int flag = 0;
	
	if (x1 >= x3) {
		t = (double)(x4 - x1) / (double)(x2 - x1); /*start from inside*/		
		flag = 0;
	} else {
		t = (double)(x3 - x1) / (double)(x2 - x1); /*start from outside*/
		flag = 1;
	}
	
	if (t < 0) {
		t = t * -1;
	}
	
	double t_2 = 1 - t; /*start from outside*/
	 
//	printf("t: %f, t2: %f\n", t, t_2);
	
	xA = x1 + t * (x2 - x1);
	yA = y1 + t * (y2 - y1);
	long xA2 = x1 + t_2 * (x2 - x1);
	long yA2 = y1 + t_2 * (y2 - y1);
	
	
	printf("%d, %d, %d, %d\n", xA, yA, xA2, yA2);
	
	if (xA >= x3 && xA <= x4 && yA >= y3 && yA <= y4) {
//		cnt += isInside(x1, y1, x2, y2, xA, yA, t1, t2, query->minT, query->maxT);
		cnt += isInside_2(x1, y1, x2, y2, xA, yA, xA2, yA2, t1, t2, query->minT, query->maxT, flag);
	}  
	
	return cnt;
}

static int isInside(long x1, long y1, long x2, long y2, long xA, long yA, long t1, long t2, long minT, long maxT)
{
	int cnt = 0;
	long inDiff_x = pow(xA - x1, 2);
	long outDiff_x = pow(x2 - xA, 2);
	long inDiff_y = pow(yA - y1, 2);
	long outDiff_y = pow(y2 - yA, 2);
	long insideLen = sqrt(inDiff_x + inDiff_y);
	long outsideLen = sqrt(outDiff_x + outDiff_y);
	long dist = insideLen + outsideLen;
	long time = t2 - t1;
	long insideTR = (time * insideLen / dist);
	long outsideTR = time - insideTR;
	
	printf("%d, %d, %d, %d, %d\n", insideLen, outsideLen, time, insideTR, outsideTR);
	
	if (insideTR >= minT && insideTR <= maxT) {
		printf("in2\n");
		cnt = 1;
	}
	
	 return cnt;
}

static int isInside_2(long x1, long y1, long x2, long y2, long xA, long yA, long xA_2, long yA_2, long t1, long t2, long minT, long maxT, int flag)
{
	int cnt = 0;
	long outDiff_x;
	long inDiff_x;
	long outDiff_y;
	long inDiff_y;
	long insideLen;
	long outsideLen;
	long dist;
	long time;
	long insideTR;
	long outsideTR;
	
	if (flag == 1) {
		outDiff_x = pow((xA - x1) + (x2 - xA_2) , 2);
		inDiff_x = pow(xA_2 - xA, 2);
	} else {
		outDiff_x = pow((x1 - xA) + (xA_2 - x2) , 2);
		inDiff_x = pow(xA_2 - xA, 2);		
	}
	
	outDiff_y = pow((yA - y1) + (y2 - yA_2), 2);
	inDiff_y = pow(yA_2 - yA, 2);
	
	insideLen = sqrt(inDiff_x + inDiff_y);
	outsideLen = sqrt(outDiff_x + outDiff_y);
	dist = insideLen + outsideLen;
	time = t2 - t1;
	insideTR = (time * insideLen / dist);
	outsideTR = time - insideTR;
	
	printf("%d, %d, %d, %d, %d\n", insideLen, outsideLen, time, insideTR, outsideTR);
	
	if (insideTR >= minT && insideTR <= maxT) {
		cnt = 1;
	}
	
	return cnt;
}
