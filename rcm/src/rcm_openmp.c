/*
***********************************
*    - Reverse Cuthill McKee -    *
*     Parallel Implementation	  *
*		   Using OpenMP           *
***********************************
*/

#include <omp.h>
#include "../inc/rcm.h"

//! Define number of threads
#define NUM_THREADS omp_get_max_threads() // Set threads as the number of cores (4 in my case)

//! Define thresholds for parallelism
#define THRES_1 2000 // Threshold for parallelization of degrees array creation
#define THRES_2 1000 // Threshold for parallelization of neighbors' searching
#define THRES_3 100	 // Threshold for parallelization of neighbors' sorting

int *rcm(int *X, int n)
{
	Queue *Q = createQueue(n); // Queue array
	Queue *R = createQueue(n); // Result array

	int *degrees = malloc(n * sizeof(int));		   // Array containing degree of all nodes
	int *last_neighbors = malloc(n * sizeof(int)); // Array containining the index of the last neighbors
	int *inserted = malloc(n * sizeof(int));	   // Shows if the node is already inserted to R or Q (0 or 1)

	//! Check for malloc failures
	if (degrees == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for degrees failed\n\n");
		exit(1);
	}
	if (last_neighbors == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for last_neighbors failed\n\n");
		exit(1);
	}
	if (inserted == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for inserted failed\n\n");
		exit(1);
	}

	//! Initialize inserted array with zeros
	for (int i = 0; i < n; i++)
		inserted[i] = 0;

	//! Initialize R array with -1
	for (int i = 0; i < n; i++)
		R->elements[i] = -1;

	//! Find degree of each node (sum of non-diagonial elements
	//! of each corresponding row). For each element, also
	//! store the index of its last neighbor for later
	//! Do it parallel only if n > 2000
	int i_;
	if (n > THRES_1)
#pragma omp parallel private(i_) num_threads(NUM_THREADS)
	{
#pragma omp for schedule(dynamic)
		for (i_ = 0; i_ < n; i_++)
		{
			int degree = 0;
			int last_neighbor = 0;

			for (int j = 0; j < n; j++)
				if (X[n * i_ + j] && (j != i_))
				{
					last_neighbor = j;
					degree++;
				}

			degrees[i_] = degree;
			last_neighbors[i_] = last_neighbor;
		}
	}
	else
		for (i_ = 0; i_ < n; i_++)
		{
			int degree = 0;
			int last_neighbor = 0;

			for (int j = 0; j < n; j++)
				if (X[n * i_ + j] && (j != i_))
				{
					last_neighbor = j;
					degree++;
				}

			degrees[i_] = degree;
			last_neighbors[i_] = last_neighbor;
		}

	//! Do the algorithm until R is full
	while (!isFull(R))
	{
		//! Find the object with minimum degree whose
		//! index has not yet been inserted to R
		int min_degree = n + 1;
		int min_degree_idx = -1;

		for (int i = 0; i < n; i++)
		{
			if ((degrees[i] < min_degree) && (inserted[i] == 0))
			{
				min_degree = degrees[i];
				min_degree_idx = i;
			}
		}

		//! Insert index of minimum degree object to R
		enqueue(R, min_degree_idx);
		inserted[min_degree_idx] = 1;
		if (degrees[min_degree_idx])
		{
			//! Insert all of its neighbors (not already inserted to R)
			//! to Q, sorted in increasing order of degree
			add_neighbors_to_queue_parallel(X, n, degrees, inserted, Q, min_degree_idx,
											last_neighbors[min_degree_idx]);

			//! While Q is not empty, extract its first node. If this
			//! node has not been inserted in R, add it to R and add
			//! its neighbors in increasing order of degree to Q
			while (!isEmpty(Q))
			{
				//! Remove the first element of Q
				int removed_item = peek(Q);
				dequeue(Q);

				//! Insert index of this element to R
				enqueue(R, removed_item);

				//! If it has neighbors, add all of them (not already inserted
				//! to R or Q) to Q, sorted in increasing order of degree
				if (degrees[removed_item])
					add_neighbors_to_queue_parallel(X, n, degrees, inserted, Q, removed_item,
													last_neighbors[removed_item]);
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
	int num_of_neigh = degrees[element_idx]; // number of neighbors
	int *neighbors = malloc(num_of_neigh * sizeof(int));
	if (neighbors == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for 'neighbors' failed\n\n");
		exit(1);
	}

	int count = 0;
	int j_;
	if (n > THRES_2)
#pragma omp parallel private(j_) num_threads(NUM_THREADS)
	{
#pragma omp for
		for (j_ = 0; j_ < last_neighbor_idx + 1; j_++)
			if ((X[n * element_idx + j_] == 1) && (j_ != element_idx))
#pragma omp critical
				neighbors[count++] = j_;
	}
	else
		for (j_ = 0; j_ < last_neighbor_idx + 1; j_++)
			if ((X[n * element_idx + j_] == 1) && (j_ != element_idx))
				neighbors[count++] = j_;

	//! Sort the neighbors in increasing order of degree using quickSort
	//! If the neighbors are more than 100, then do it in parallel, using 2 threads
	if (num_of_neigh > THRES_3)
	{
		int pi = partition(neighbors, degrees, 0, num_of_neigh - 1);
#pragma omp parallel sections
		{
#pragma omp section
			quickSort(neighbors, degrees, 0, pi - 1); // Thread 1
#pragma omp section
			quickSort(neighbors, degrees, pi + 1, num_of_neigh - 1); // Thread 2
		}
	}
	else
		quickSort(neighbors, degrees, 0, num_of_neigh - 1);

	//! Insert all of its neighbors (not already inserted to R or Q) to Q
	for (int i = 0; i < num_of_neigh; i++)
		if (!inserted[neighbors[i]])
		{
			enqueue(Q, neighbors[i]);
			inserted[neighbors[i]] = 1;
		}

	free(neighbors);
}