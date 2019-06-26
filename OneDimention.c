#include <stdio.h>
#include <stdlib.h>

//a structure for locations
struct node 
{
	float x, y, t;
};
//a structure for each path
struct path
{
	struct node pathNodes[10000];
};
//a structure for each query
struct query 
{
	float xL, yL, xR, yR, minT, maxT;  
};

//function prototype
void oneDimention_multiPath(struct path paths[100],struct query queries[100], int numberOfpaths, int numberOfNodes, int numberOfQueries);//for multi path
void oneDimention_singlePath(struct path paths[100],struct query queries[100], int numberOfNodes, int numberOfQueries);//just for one path


int main() 
{
	int i, j;//counters
	int numberOfPaths, numberOfNodes, numberOfQueries;
	int multiPath = 0;//a variable to specify we we have one path or more
	
//	printf("single path: 0 \nmulti path: 1\n");
//	scanf("%d", &multiPath);	
		
	scanf("%d", &numberOfPaths);
	struct path paths[numberOfPaths];
	
	//get locations for each path
	for (i = 0; i < numberOfPaths; i++)
	{
		scanf("%d", &numberOfNodes);
		for (j = 0; j < numberOfNodes; j++)
		{
			scanf("%f",&paths[i].pathNodes[j].t);						
			scanf("%f",&paths[i].pathNodes[j].x);
			scanf("%f",&paths[i].pathNodes[j].y);
		}
	}
	
	scanf("%d", &numberOfQueries);
	struct query queries[numberOfQueries];
	
	//get queries
	for (i = 0; i < numberOfQueries; i++) 
	{
		scanf("%f", &queries[i].xL);
		scanf("%f", &queries[i].yL);
		scanf("%f", &queries[i].xR);
		scanf("%f", &queries[i].yR);
		scanf("%f", &queries[i].minT);
		scanf("%f", &queries[i].maxT);
	}
	
	if (multiPath == 0)//if we have one path
		oneDimention_singlePath(paths ,queries, numberOfNodes, numberOfQueries);
// 	else if (multiPath == 1)//if we have more than one path
//	    oneDimention_multiPath(paths ,queries, numberOfPaths, numberOfNodes, numberOfQueries);
}

void oneDimention_singlePath(struct path paths[100],struct query queries[100], int numberOfNodes, int numberOfQueries) 
{
	int i, j, z;
	int count[100] = {0};
	
	for (j = 0; j < numberOfQueries; j++)//for each query
	{
		for (z = 0; z < numberOfNodes - 1; z++)//for each two node of a way  
		{
			if (paths[0].pathNodes[z].x < paths[0].pathNodes[z + 1].x)
			{	
				if (paths[0].pathNodes[z].x < queries[j].xR && paths[0].pathNodes[z + 1].x > queries[j].xL)				
				{
					count[j]++;
				}
			}
			else if (paths[0].pathNodes[z].x > paths[0].pathNodes[z + 1].x)
			{
				if (paths[0].pathNodes[z].x > queries[j].xL && paths[0].pathNodes[z + 1].x < queries[j].xR)				
				{
					count[j]++;
				}
			}
		}
	}
	
	//print answers
	for (i = 0; i < numberOfQueries; i++) 
	{
		printf("%d\n", count[i]);
	}
}

void oneDimention_multiPath(struct path paths[100],struct query queries[100], int numberOfpaths, int numberOfNodes, int numberOfQueries) 
{
	int i, j, z;//counters
	int n = numberOfpaths;
	int count[100] = {0};//store queries answers
	
	for (i = 0; i < n; i++)//for each path 
	{
		for (j = 0; j < numberOfQueries; j++)//for each query
		{
			for (z = 0; z < numberOfNodes - 1; z++)//for each two node of a way 
			{
				if (paths[i].pathNodes[z].x < paths[i].pathNodes[z + 1].x)
				{	
					if (paths[i].pathNodes[z].x < queries[j].xR && paths[i].pathNodes[z + 1].x > queries[j].xL)				
					{
						count[j]++;
					}
				}
				else if (paths[i].pathNodes[z].x > paths[i].pathNodes[z + 1].x)
				{
					if (paths[i].pathNodes[z].x > queries[j].xL && paths[i].pathNodes[z + 1].x < queries[j].xR)				
					{
						count[j]++;
					}
				}
			}
		}
	}
	
	//print answers
	for (i = 0; i < numberOfQueries; i++) 
	{
		printf("%d\n", count[i]);
	}
}
