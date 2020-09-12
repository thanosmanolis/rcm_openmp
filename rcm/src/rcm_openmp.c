/*
***********************************
*    - Reverse Cuthill McKee -    *
*     Parallel Implementation	  *
*		   Using OpenMP           *
***********************************
*/

#include <omp.h>
#include "../inc/rcm.h"

int *rcm(int *X, int n)
{
	Queue *Q = createQueue(n*n);	// Queue array
	Queue *R = createQueue(n);		// Result array

	int *degrees = malloc(n * sizeof(int));			// Array containing degree of all nodes
	int *last_neighbors = malloc(n * sizeof(int)); 	// Array containining the index of the last neighbors
	int *inserted = malloc(n * sizeof(int));		// Shows if the node is already inserted to R (0 or 1)

	//! Check for malloc failures
	if( degrees == NULL )
    {
        printf(RED "Error:" RESET_COLOR " Memory allocation for degrees failed\n\n");
		exit(1);
    }
	if( last_neighbors == NULL )
    {
        printf(RED "Error:" RESET_COLOR " Memory allocation for last_neighbors failed\n\n");
		exit(1);
    }
	if( inserted == NULL )
    {
        printf(RED "Error:" RESET_COLOR " Memory allocation for inserted failed\n\n");
		exit(1);
    }
	
	//! Initialize inserted array with zeros
	for(int i=0; i<n; i++)
		inserted[i] = 0;

	//! Initialize R array with -1
	for(int i=0; i<n; i++)
		R->elements[i] = -1;

	//! Find degree of each node (sum of non-diagonial elements
	//! of each corresponding row). For each element, also
	//! store the index of its last neighbor for later
	//! Do it parallel only if n > 2000
	int i_;
	if(n>2000)
		#pragma omp parallel private(i_) num_threads(NUM_THREADS)
		{
			#pragma omp for schedule(dynamic)
				for(i_=0; i_<n; i_++)
				{	
					int degree = 0;
					int last_neighbor = 0;

					for(int j=0; j<n; j++)
						if( X[n*i_ + j] && (j != i_) )
						{
							last_neighbor = j;
							degree++;
						}
					
					degrees[i_] = degree;
					last_neighbors[i_] = last_neighbor;
				}
		}
	else
		for(i_=0; i_<n; i_++)
		{	
			int degree = 0;
			int last_neighbor = 0;

			for(int j=0; j<n; j++)
				if( X[n*i_ + j] && (j != i_) )
				{
					last_neighbor = j;
					degree++;
				}
			
			degrees[i_] = degree;
			last_neighbors[i_] = last_neighbor;
		}

	//! Do the algorithm until R is full
	while(!isFull(R))
	{
		//! Find the object with minimum degree whose  
		//! index has not yet been inserted to R
		int min_degree = n + 1;
		int min_degree_idx = -1;

		for(int i=0; i<n; i++)
		{
			if( (degrees[i] < min_degree) && (inserted[i] == 0) )
			{
				min_degree = degrees[i];
				min_degree_idx = i;
			}
		}

		//! Insert index of minimum degree object to R	
		enqueue(R, min_degree_idx);
		inserted[min_degree_idx] = 1;

		//! Insert all of its neighbors (not already inserted to R)
		//! to Q, sorted in increasing order of degree
		add_neighbors_to_queue_parallel(X, n, degrees, inserted, Q,
					min_degree_idx, last_neighbors[min_degree_idx]);
		
		//! While Q is not empty, extract its first node. If this
		//! node has not been inserted in R, add it to R and add
		//! its neighbors in increasing order of degree to Q
		while(!isEmpty(Q))
		{
			//! Remove the first element of Q
			int removed_item = peek(Q);
			dequeue(Q);
			
			//! Check if it has been already inserted to R
			if(!inserted[removed_item])
			{
				//! Insert index of this element to R
				enqueue(R, removed_item);
				inserted[removed_item] = 1;
				
				//! Insert all of its neighbors (not already inserted to R)
				//! to Q, sorted in increasing order of degree
				add_neighbors_to_queue_parallel(X, n, degrees, inserted, Q,  
								removed_item, last_neighbors[removed_item]);
			}
		}
	}
	
	//! Reverse R array
	reverse_array(R->elements, n);

	//! Free allocated memory
	free(Q);
	free(degrees);
	free(inserted);
	free(last_neighbors);

	return R->elements;
}

/*
***********************************************
*    Function that adds neighbors to queue    *
***********************************************
*/

void add_neighbors_to_queue_parallel(int *X, int n, int *degrees, int *inserted, 
								Queue *Q, int element_idx, int last_neighbor_idx)
{
	//! Find all of its neighbors and store them to an array
	//! Do it parallel only if n > 2000
	int num_of_neigh = degrees[element_idx];	// number of neighbors
	int *neighbors = malloc(num_of_neigh * sizeof(int));
	if( neighbors == NULL )
    {
        printf(RED "Error:" RESET_COLOR " Memory allocation for 'neighbors' failed\n\n");
		exit(1);
    }
	
	int count = 0;
	int j_;
	if(n > 1000)
		#pragma omp parallel private(j_) num_threads(NUM_THREADS)
		{
			#pragma omp for
			for(j_=0; j_<last_neighbor_idx+1; j_++)
				if( (X[n*element_idx + j_] == 1) && (j_ != element_idx) )
					#pragma omp critical
						neighbors[count++] = j_;
		}
	else
		for(j_=0; j_<last_neighbor_idx+1; j_++)
			if( (X[n*element_idx + j_] == 1) && (j_ != element_idx) )
				neighbors[count++] = j_;

	//! Sort the neighbors in increasing order of degree using quickSort
	quickSort(neighbors, degrees, 0, num_of_neigh - 1);
	
	//! Insert all of its neighbors (not already inserted to R) to Q
	for(int i=0; i<num_of_neigh; i++)
		if(!inserted[neighbors[i]])
			enqueue(Q, neighbors[i]);

	free(neighbors);
}

/*
************************************************************
*    Function that reverses the values of a given array    *
************************************************************
*/

void reverse_array(int *X, int n)
{
	for(int i=0; i<n/2; i++)
		swap(&X[i], &X[n-i-1]);
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
	if( Q->elements == NULL )
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
   	if(Q->size == Q->capacity)
		printf("Queue is Full\n");
   	else
   	{
      	Q->size++;
      	Q->rear = Q->rear + 1;
		if(Q->rear == Q->capacity)
		  Q->rear = 0;

		//! Insert the element in its rear side
		Q->elements[Q->rear] = element;	
   	}
}

void dequeue(Queue *Q)
{	
	//! If queue size is zero then it is empty. So we cannot pop
	if(Q->size==0)
		printf("Queue is Empty\n");
	else // Remove the element and increment front by one
	{
		Q->size--;
		Q->front++;
		
		if(Q->front == Q->capacity)
			Q->front = 0;
	}
}

int peek(Queue *Q)
{
	if(Q->size==0)
	{
		printf("Queue is Empty\n");
		exit(0);
	}

	//! Return the element which is at the front
	return Q->elements[Q->front];
}

int	isEmpty(Queue *Q)
{
	if(Q->size==0)
		return 1;
	else
		return 0;
}

int	isFull(Queue *Q)
{
	if(Q->size == Q->capacity)
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

int partition (int arr1[], int arr2[], int low, int high) 
{ 
    int pivot = arr2[ arr1[high] ];	// pivot 
    int i = low - 1;  				// Index of smaller element 
	
    for(int j=low; j<=high-1; j++) 
    { 
        //! If current element is smaller than the pivot 
        if (arr2[ arr1[j] ] < pivot) 
        { 
            i++;	// increment index of smaller element 
            swap(&arr1[i], &arr1[j]); 
        } 
    } 

	swap(&arr1[i+1], &arr1[high]); 

    return(i + 1); 
} 

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
}