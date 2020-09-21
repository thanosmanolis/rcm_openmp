#ifndef RCM_H
#define RCM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include <sys/time.h>
#include <sys/times.h>

//! Define some print color values
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define GREEN_BOLD "\033[1;32m"
#define YELLOW "\033[0;33m"
#define RESET_COLOR "\033[0m"

//! Define number of threads
#define NUM_THREADS 4

/*
********************************************************
*    @file   rcm.h                                     *
*    @author Thanos Manolis <thanos.m14@gmail.coms>    *
*    @date   Wed Aug 26 15:11:30 2020                  *
*    @brief  Main utility functions                    *
********************************************************
*/

/*
************************************************************************
*    --- Reverse Cuthill-McKee Algorithm ---                           *
*                                                                      *
*    Pass through a 1D array (representing a 2D array in row-major)    * 
*    along with its size (n) and this function will implement the      *
*    RCM algorithm to it, returning the permutation array needed       *
*    for the bandwidth reduction through graph modification            *
*                                                                      *
*    - param X    1D array          [n-by-n]                           *
*    - param n    Size of Matrix    [scalar]                           *          
*                                                                      *
*    NOTE: Matrix is stored in row-major format                        *
************************************************************************
*/

int *rcm(int *X, int n);

/*
**********************************************************************
*    --- Queue implementation ---                                    *
*                                                                    *
*    A set of functions to demonstrate the functioning of a queue    *
*                                                                    *
*    - createQueue() Initialize queue and its values                 *
*    - enqueue()     Insert an item                                  *
*    - dequeue()     Remove an item                                  *
*    - peek()        See first element                               *
*    - isEmpty()     Check if queue is empty                         *
*    - isFull()      Check if queue is full                          *
**********************************************************************
*/

typedef struct Queue
{
	int capacity;
	int size;
	int front;
	int rear;
	int *elements;
} Queue;

Queue *createQueue(int max_elements);
void enqueue(Queue *Q, int element);
void dequeue(Queue *Q);
int peek(Queue *Q);
int isEmpty(Queue *Q);
int isFull(Queue *Q);

/*
*****************************************************************************
*    --- Add neighbors to queue ---                                         *
*                                                                           *
*    This function implements a step of RCM algorithm, where we             *
*    insert the neighbors (not already inserted to R) of an                 *
*    element to Q, sorted in increasing order of degree.                    *
*                                                                           *
*    - param X             1D array                                         *
*    - param n             Size of Matrix                                   * 
*    - param degrees       Array containing degree of all nodes             *
*    - param inserted      Shows if the node is already inserted to R       *    
*    - param Q             Queue array          [n-by-n]                    *
*    - param element_idx   Element of which neighbors are to be inserted    *
*                                                                           *
*    The parallel version of it, also has this argument                     *
*                                                                           *
*    - param last_neigbor_idx   Index of the element's last neighbor        *
*****************************************************************************
*/
void add_neighbors_to_queue(int *X, int n, int *degrees,
							int *inserted, Queue *Q, int element_idx);

void add_neighbors_to_queue_parallel(int *X, int n, int *degrees, int *inserted,
									 Queue *Q, int element_idx, int last_neighbor_idx);

/*
**************************************************************************
*    --- QuickSort implementation ---                                    *
*                                                                        *
*    Sort a given array, depending on the values of a different array    *
*                                                                        *
*    - arr1[]  Array to be sorted                                        *
*    - arr2[]  Array on which the sorting depends                        *
*    - low     Starting index                                            *
*    - high    Ending index                                              *
**************************************************************************
*/

int partition(int arr1[], int arr2[], int low, int high);
void quickSort(int arr1[], int arr2[], int low, int high);
void swap(int *a, int *b);

/*
**********************************************************************
*    --- Graph implementation ---                                    *
*                                                                    *
*    A set of functions to demonstrate the functioning of a graph    *
*    - createNode()     Create a node                                *
*    - createGraph()    Create a graph                               *
*    - addEdge()        Add an edge                                  *
*    - printGraph()     Print the graph                              *
**********************************************************************
*/

typedef struct node
{
	int vertex;
	struct node *next;
} node;

typedef struct Graph
{
	int numVertices;
	node **adjLists;
} Graph;

Graph *createGraph(int vertices);
node *createNode(int v);
void addEdge(Graph *graph, int s, int d);
void printGraph(Graph *graph);

/*
************************************************************************
*    --- Helper Functions ---                                          *
*                                                                      *
*    - create_matrix()  Create a a matrix of given size and density    *
*    - reverse_array()  Reverse the values of a given array            *
*    - print_array()    Print a 1d array                               *
*    - print_array_2d() Print a 2D array                               *
*    - calc_bandwidth() Calculate the bandwidth of a given matrix      *
*                                                                      *
*    NOTE: All matrices/arrays are stored in 1D arrays represented     *
*          in row-major format                                         *
************************************************************************
*/

void reverse_array(int *X, int n);
void print_array(int *X, int n);
void print_array_2d(int *X, int n);
int calc_bandwidth(int *X, int n);

#endif