#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

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

const R = 6371; //a constant as radius of earth for changing location to x and y

/*convert variables that are read from file from string to double*/
static double converted_substring(char src[], int start, int end)  
{
	char dest[end - 1];
	double converted;
	int i;
	
	for (i = 0; i < end; i++) {
		dest[i] = src[i + start];
	}
	
	sscanf(dest, "%lf", &converted);
	
	return converted;
}

static int answer(struct path *paths, long llx, long lly, long urx, long ury, long minT, long maxT)
{
	int cnt = 0;
	int i;
	
	for (i = 0; i < paths->n - 1; i++) {
		cnt += sensitive_count_visits(&paths->nodes[i], &paths->nodes[i + 1], llx, lly, urx, ury, minT, maxT); 
	}
	
	return cnt;
}

 int sensitive_count_visits(struct node *beginNode, struct node *endNode, long llx, long lly, long urx, long ury, long minT, long maxT)
{
	int cnt = 0;
	int x1 = beginNode->x;
	int y1 = beginNode->y;
	int x2 = endNode->x;
	int y2 = endNode->y;
	int t1 = beginNode->t;
	int t2 = endNode->t;
	
	if (x2 >= llx && x2 <= urx && y2 >= lly && y2 <= ury) { /*end point is in query or not*/
		return 0;
	} else if (x1 < llx && x2 <= llx) { /*two points are out-left side of query*/
		return 0;
	} else if (x1 > urx && x2 >= urx) { /*two points are out-right side of query*/
		return 0;
	} else if (y1 < lly && y2 <= lly) { /*two points are out-down side of query*/
		return 0;
	} else if (y1 > ury && y2 >= ury) { /*two points are out-up side of query*/
		return 0;
	} else { /*line will intersect the query*/
		cnt += findIntersection(x1, y1, t1, x2, y2 , t2, llx, lly, urx, ury, minT, maxT);
	}
		
	return cnt;
}

int findIntersection(long x1, long y1, long t1, long x2, long y2, long t2 , long llx, long lly, long urx, long ury, long minT, long maxT) {
	double isIntercepted_1, isIntercepted_2, isIntercepted_3, isIntercepted_4;
	int i = 0, j = 0, cnt = 0;
	long x[2], y[2];
	
	/*these pararmeter's value should be between 0 & 1*/
	isIntercepted_1 = (double) ((ury - lly)*(x1 - llx) + (llx - llx)*(y1 - ury)) / (double)((llx - llx)*(y1 - y2) - (x1 - x2)*(lly - ury));
	isIntercepted_2 = (double) ((lly - lly)*(x1 - llx) + (urx - llx)*(y1 - lly)) / (double)((urx - llx)*(y1 - y2) - (x1 - x2)*(lly - lly));
	isIntercepted_3 = (double) ((lly - ury)*(x1 - urx) + (urx - urx)*(y1 - lly)) / (double)((urx - urx)*(y1 - y2) - (x1 - x2)*(ury - lly));
	isIntercepted_4 = (double) ((ury - ury)*(x1 - urx) + (llx - urx)*(y1 - ury)) / (double)((llx - urx)*(y1 - y2) - (x1 - x2)*(ury - ury));
	
	if (isIntercepted_1 >= 0.0000001 && isIntercepted_1 <= 1) {
		x[i] = x1 + (isIntercepted_1 * (x2 - x1));
		y[i] = y1 + (isIntercepted_1 * (y2 - y1));
		i++;
	}
	if (isIntercepted_2 >= 0.0000001 && isIntercepted_2 <= 1) {
		x[i] = x1 + (isIntercepted_2 * (x2 - x1));
		y[i] = y1 + (isIntercepted_2 * (y2 - y1));
		i++;
	}
	if (isIntercepted_3 >= 0.0000001 && isIntercepted_3 <= 1) {
		x[i] = x1 + (isIntercepted_3 * (x2 - x1));
		y[i] = y1 + (isIntercepted_3 * (y2 - y1)); 
		i++;
	}
	if (isIntercepted_4 >= 0.0000001 && isIntercepted_4 <= 1) {
		x[i] = x1 + (isIntercepted_4 * (x2 - x1));
		y[i] = y1 + (isIntercepted_4 * (y2 - y1));
		i++;
	}
	
	long duration = t2 - t1;
	 
	if (i == 1) { /*start point is inside the query*/
		cnt += isInside_1ed(x1, y1, x2, y2, x[0], y[0], duration, minT, maxT);
	} 
	if (i == 2) { /*start point is outside the query*/
		cnt += isInside_2ed(x1, y1, x2, y2, x[0], y[0], x[1], y[1], duration, minT, maxT);
	} 
	
	return cnt;
}

int isInside_1ed(long x1, long y1, long x2, long y2, long x_intersection, long y_intersection, long duration, long minT, long maxT)
{
	long x_inside = x1 - x_intersection;
	long x_outside = x2 - x_intersection;
	long y_inside = y1 - y_intersection;
	long y_outside = y2 - y_intersection;
	long inside = sqrt(pow((double)x_inside, 2) + pow((double)y_inside, 2));
	long outside = sqrt(pow((double)x_outside, 2) + pow((double)y_outside, 2));
	long dist = inside + outside;
	long insideTR = (inside * duration) / dist;
	
	if (insideTR >= minT && insideTR <= maxT) {
		return 1;
	}
	
	return 0;
}

int isInside_2ed(long x1, long y1, long x2, long y2, long x_intersection1, long y_intersection1,
				 long x_intersection2, long y_intersection2, long duration, long minT, long maxT)
{
	long x_inside = abs((x_intersection1 - x_intersection2)) ;
	long y_inside = abs(y_intersection1 - y_intersection2);
	
	long inside = sqrt(pow((double)x_inside, 2) + pow((double)y_inside, 2));
	 
	long dist = sqrt(pow((double)(x1 - x2),2) + pow((double)(y1 - y2),2));
	
	long insideTR = (inside * duration) / dist;
	
	if (insideTR >= minT && insideTR <= maxT) {
		return 1;
	}
	
	return 0;
}

int main() 
{
	int npaths, nqueries;
	struct path *paths;		/* input trajectories */
	int restrict_duration = 0;	/* restrict the duration of visits */
	int i, j, z = 0;
	FILE *file;
	char ch[100000];
	
	/*reading input trajectories from file*/
	file = fopen("test_file.txt", "r");
	
	if (file == NULL) {
		printf("Null");
		return 1;
	}	
	fscanf(file, "%s", &ch);
	npaths  = converted_substring(ch, 0, 2);
	paths = malloc(npaths * sizeof(paths[0]));
	fscanf(file, "%s", &ch);
	paths[0].n  = converted_substring(ch, 0, 4);
	paths[0].nodes = malloc(paths[0].n * sizeof(paths[0].nodes[0]));
	
	while (fscanf(file, "%*s %s", &ch) != EOF) {		
		
		int hour = converted_substring(ch, 8, 2);
		int min = converted_substring(ch, 3, 2);
		int sec = converted_substring(ch, 6, 2);
		double longtitude = converted_substring(ch, 9, 9) * (M_PI / 180);
		double latitude = converted_substring(ch, 19, 9) * (M_PI / 180);
			
		paths[0].nodes[z].t = (hour * 3600) + (min * 60) + sec;
		paths[0].nodes[z].x = R * cos(longtitude) * -1;
		paths[0].nodes[z].y = R * cos(latitude);			
		
		z++;
	}
	/*end of reading file*/
	
	scanf("%d", &nqueries);
	for (i = 0; i < nqueries; i++) {
		int visits = 0;
		for (j = 0; j < npaths; j++) {
			long llx, lly, urx, ury;
			long minT, maxT;
			scanf("%ld %ld %ld %ld", &llx, &lly, &urx, &ury);
			scanf("%ld %ld", &minT, &maxT);
			visits += answer(&paths[j], llx, lly, urx, ury, minT, maxT);			
		}
		printf("%d\n", visits);
	}	
}
