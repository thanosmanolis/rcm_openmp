/*
***************************************
*      - Reverse Cuthill McKee -      *
*      Sequential Implementation      *
***************************************
*/

#include "../inc/rcm.h"

int *rcm(int *X, int n)
{
	Queue *Q = createQueue(n);				 // Queue array
	Queue *R = createQueue(n);				 // Result array
	int *degrees = malloc(n * sizeof(int));	 // Array containing degree of all nodes
	int *inserted = malloc(n * sizeof(int)); // Shows if the node is already inserted to R or Q (0 or 1)

	//! Check for malloc failures
	if (degrees == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for 'degrees' failed\n\n");
		exit(1);
	}
	if (inserted == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for 'inserted' failed\n\n");
		exit(1);
	}

	//! Initialize inserted array with zeros
	for (int i = 0; i < n; i++)
		inserted[i] = 0;

	//! Initialize R array with -1
	for (int i = 0; i < n; i++)
		R->elements[i] = -1;

	//! Find degree of each node (sum of non-diagonial
	//! elements of each corresponding row)
	for (int i = 0; i < n; i++)
	{
		int degree = 0;

		for (int j = 0; j < n; j++)
			if (X[n * i + j] && (j != i))
				degree++;

		degrees[i] = degree;
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
			//! Insert all of its neighbors (not already inserted to R or Q)
			//! to Q, sorted in increasing order of degree
			add_neighbors_to_queue(X, n, degrees, inserted, Q, min_degree_idx);

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
					add_neighbors_to_queue(X, n, degrees, inserted, Q, removed_item);
			}
		}
	}

	//! Reverse R array
	reverse_array(R->elements, n);

	//! Free allocated memory
	free(Q);
	free(degrees);
	free(inserted);

	return R->elements;
}

/*
***********************************************
*    Function that adds neighbors to queue    *
***********************************************
*/

void add_neighbors_to_queue(int *X, int n, int *degrees,
							int *inserted, Queue *Q, int element_idx)
{
	//! Find all of its neighbors and store them to an array
	int num_of_neigh = degrees[element_idx]; // number of neighbors
	int *neighbors = malloc(num_of_neigh * sizeof(int));
	if (neighbors == NULL)
	{
		printf(RED "Error:" RESET_COLOR " Memory allocation for 'neighbors' failed\n\n");
		exit(1);
	}

	int count = 0;
	for (int j = 0; j < n; j++)
	{
		if ((X[n * element_idx + j] == 1) && (j != element_idx))
		{
			neighbors[count++] = j;
			if (count == num_of_neigh)
				break;
		}
	}

	//! Sort the neighbors in increasing order of degree using quickSort
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