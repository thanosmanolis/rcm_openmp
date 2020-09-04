/*
***************************************************
*    Some extra functions used at this project    *
***************************************************
*/

#include "../inc/rcm.h"

/*
******************************
*    Graph Implementation    *
******************************
*/

node *createNode(int v) 
{
	node* newNode = malloc(sizeof(node));
	newNode->vertex = v;
	newNode->next = NULL;

	return newNode;
}

Graph *createGraph(int vertices) 
{
	Graph* graph = malloc(sizeof(Graph));
	graph->numVertices = vertices;

	graph->adjLists = malloc(vertices * sizeof(node*));

	for(int i=0; i<vertices; i++)
		graph->adjLists[i] = NULL;

	return graph;
}

void addEdge(Graph* graph, int s, int d) 
{
	//! Add edge from s to d
	node* newNode = createNode(d);
	newNode->next = graph->adjLists[s];
	graph->adjLists[s] = newNode;

	//! Add edge from d to s
	newNode = createNode(s);
	newNode->next = graph->adjLists[d];
	graph->adjLists[d] = newNode;
}

void printGraph(Graph* graph) 
{
	for(int v=0; v < graph->numVertices; v++) 
	{
		node* temp = graph->adjLists[v];
		printf("\n Vertex %d\n: ", v);

		while (temp) 
		{
			printf("%d", temp->vertex);
			temp = temp->next;
			if(temp)
				printf(" -> ");
		}
		printf("\n");
	}
	printf("\n");
}

/*
**************************
*    Helper Functions    *
**************************
*/

void print_array(int *X, int n)
{
	for(int i=0; i<n; i++)
		printf("%d ", X[i]);
    printf("\n\n");
}

void print_array_2d(int *X, int n)
{
	for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            if(X[n*i + j])
				printf(GREEN "%d " RESET_COLOR, X[n*i + j]);
			else	
				printf(RED "%d " RESET_COLOR, X[n*i + j]);
        printf("\n");
    }
    printf("\n");
}

int calc_bandwidth(int *X, int n)
{
    int band_hi = 0;
	int band_lo = 0;
	int temp;

    for(int i=0; i<n; i++)
    {        		
		temp = 0;
        for(int j=n-1; j>i; j--)
            if(X[n*i + j] != 0)
            {
                temp = j - i;
                break;
            }
            else
                continue;
        
        if(temp > band_hi)
            band_hi = temp; 

		temp = 0;
        for(int j=0; j<i; j++)
            if(X[n*i + j] != 0)
            {
                temp = i - j;
                break;
            }
            else
                continue;

        if(temp > band_lo)
            band_lo = temp; 
    }

    int bandwidth = band_lo + band_hi + 1;

    return bandwidth;
}