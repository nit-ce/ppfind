#include <stdio.h>

struct node 
{
	int b, e, key, bIndex, eIndex, count;	
};

int main() 
{
	struct node v[1000];
	int i;
	int arr[] = {-100000, 1, 5, 10, 100000};
	int arrLenght = 5;

	printf("----------------------------\ninputs\n");
	for (i = 0; i < arrLenght; i++)
	{
		printf("%d\t", arr[i]);
	}
	
	v[0].b = arr[0];
	v[0].e = arr[arrLenght - 1];
	v[0].bIndex = 0;
	v[0].eIndex = arrLenght - 1;
	v[0].count = 0;
	
	int temp = v[0].bIndex + v[0].eIndex;
	if (temp % 2 != 0)
	{
		v[0].key = (temp / 2) + 1;
	}
	else 
	{
		v[0].key = temp / 2;
	}
	
	printf("\n------------------------\ninit\n");
	printf("%d\t %d\t %d\t %d\t %d\n", v[0].b, v[0].e, v[0].bIndex, v[0].eIndex, v[0].key);
	
	int j = 1;
	i = 0;
	for (;;) 
	{
		v[j].b = v[i].b;
		v[j].e = arr[v[i].key];
		v[j].bIndex = v[i].bIndex;
		v[j].eIndex = v[i].key;
		v[j].count = 0;
		
		temp = v[j].bIndex + v[j].eIndex;
		if (temp % 2 != 0)
		{
			v[j].key = (temp / 2) + 1;
		}
		else 
		{
			v[j].key = temp / 2;
		}
		
		if ((v[j].b == v[i].b) && (v[j].e == v[i].e))
		{
			break;
		}
			
		j += 1;
		v[j].b = arr[v[i].key];
		v[j].e = v[i].e;
		v[j].bIndex = v[i].key;
		v[j].eIndex = v[i].eIndex;
		v[j].count = 0;
			
		temp = v[j].bIndex + v[j].eIndex;
		if (temp % 2 != 0)
		{
			v[j].key = (temp / 2) + 1;
		}
		else 
		{
			v[j].key = temp / 2;
		}
		
//		if ((v[j].b == v[i].b) && (v[j].e == v[i].e))
//		{
//			break;
//		}
		
		i++;
		j++;
	}	
	
	printf("\n--------------------------------\noutput\n");
	for (i = 0; i < j; i++)
	{
		printf("%d\t %d\t %d\t %d\t %d\n", v[i].b, v[i].e, v[i].bIndex, v[i].eIndex, v[i].key);
	}
}
