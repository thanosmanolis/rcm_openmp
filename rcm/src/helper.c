/*
***************************************************
*    Some extra functions used at this project    *
***************************************************
*/

#include "../inc/rcm.h"

/*
************************************************************
*    Function that reverses the values of a given array    *
************************************************************
*/

void reverse_array(int *X, int n)
{
	for (int i = 0; i < n / 2; i++)
		swap(&X[i], &X[n - i - 1]);
}

/*
******************************
*    Queue implementation    *
******************************
*/

Queue *createQueue(int max_elements)
{
	Queue *Q;
	Q = malloc(sizeof(Queue));

	Q->elements = malloc(max_elements * sizeof(int));
	if (Q->elements == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for Q->elements failed\n\n");
		exit(1);
	}

	Q->size = 0;
	Q->capacity = max_elements;
	Q->front = 0;
	Q->rear = -1;

	return Q;
}

void enqueue(Queue *Q, int element)
{
	//! If the queue is full, we cannot push an element
	//! into it as there is no space for it
	if (Q->size == Q->capacity)
		printf("Queue is Full\n");
	else
	{
		Q->size++;
		Q->rear = Q->rear + 1;
		if (Q->rear == Q->capacity)
			Q->rear = 0;

		//! Insert the element in its rear side
		Q->elements[Q->rear] = element;
	}
}

void dequeue(Queue *Q)
{
	//! If queue size is zero then it is empty. So we cannot pop
	if (Q->size == 0)
		printf("Queue is Empty\n");
	else // Remove the element and increment front by one
	{
		Q->size--;
		Q->front++;

		if (Q->front == Q->capacity)
			Q->front = 0;
	}
}

int peek(Queue *Q)
{
	if (Q->size == 0)
	{
		printf("Queue is Empty\n");
		exit(0);
	}

	//! Return the element which is at the front
	return Q->elements[Q->front];
}

int isEmpty(Queue *Q)
{
	if (Q->size == 0)
		return 1;
	else
		return 0;
}

int isFull(Queue *Q)
{
	if (Q->size == Q->capacity)
		return 1;
	else
		return 0;
}

/*
**********************************
*    QuickSort implementation    *
**********************************
*/

void quickSort(int arr1[], int arr2[], int low, int high)
{
	if (low < high)
	{
		//! pi is partitioning index, arr1[p] is now at right place
		int pi = partition(arr1, arr2, low, high);

		// Separately sort elements before partition and after partition
		quickSort(arr1, arr2, low, pi - 1);
		quickSort(arr1, arr2, pi + 1, high);
	}
}

int partition(int arr1[], int arr2[], int low, int high)
{
	int pivot = arr2[arr1[high]]; // pivot
	int i = low - 1;			  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		//! If current element is smaller than the pivot
		if (arr2[arr1[j]] < pivot)
		{
			i++; // increment index of smaller element
			swap(&arr1[i], &arr1[j]);
		}
	}

	swap(&arr1[i + 1], &arr1[high]);

	return (i + 1);
}

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/*
******************************
*    Graph Implementation    *
******************************
*/

node *createNode(int v)
{
	node *newNode = malloc(sizeof(node));
	newNode->vertex = v;
	newNode->next = NULL;

	return newNode;
}

Graph *createGraph(int vertices)
{
	Graph *graph = malloc(sizeof(Graph));
	graph->numVertices = vertices;

	graph->adjLists = malloc(vertices * sizeof(node *));

	for (int i = 0; i < vertices; i++)
		graph->adjLists[i] = NULL;

	return graph;
}

void addEdge(Graph *graph, int s, int d)
{
	//! Add edge from s to d
	node *newNode = createNode(d);
	newNode->next = graph->adjLists[s];
	graph->adjLists[s] = newNode;

	//! Add edge from d to s
	newNode = createNode(s);
	newNode->next = graph->adjLists[d];
	graph->adjLists[d] = newNode;
}

void printGraph(Graph *graph)
{
	for (int v = 0; v < graph->numVertices; v++)
	{
		node *temp = graph->adjLists[v];
		printf("\n Vertex %d\n: ", v);

		while (temp)
		{
			printf("%d", temp->vertex);
			temp = temp->next;
			if (temp)
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
	for (int i = 0; i < n; i++)
		printf("%d ", X[i]);
	printf("\n\n");
}

void print_array_2d(int *X, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			if (X[n * i + j])
				printf(GREEN "%d " RESET_COLOR, X[n * i + j]);
			else
				printf(RED "%d " RESET_COLOR, X[n * i + j]);
		printf("\n");
	}
	printf("\n");
}

int calc_bandwidth(int *X, int n)
{
	int band_hi = 0;
	int band_lo = 0;
	int temp;

	for (int i = 0; i < n; i++)
	{
		temp = 0;
		for (int j = n - 1; j > i; j--)
			if (X[n * i + j] != 0)
			{
				temp = j - i;
				break;
			}
			else
				continue;

		if (temp > band_hi)
			band_hi = temp;

		temp = 0;
		for (int j = 0; j < i; j++)
			if (X[n * i + j] != 0)
			{
				temp = i - j;
				break;
			}
			else
				continue;

		if (temp > band_lo)
			band_lo = temp;
	}

	int bandwidth = band_lo + band_hi + 1;

	return bandwidth;
}