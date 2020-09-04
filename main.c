/*
***************************************
*      - Reverse Cuthill McKee -      *
*            Main function        	  *
***************************************
*/  

#include "rcm.h"

int main(int argc, char *argv[])
{
	int n;
    double density;

    if(argc > 1)
    {
        n = atoi(argv[1]);  		// # size of matrix (n*n)
        density = atof(argv[2]);  	// # density of matrix
    }else
    {
		n = 8;			// default value for n
        density = 1;	// default value for density
    }

	printf(YELLOW "\nn: %d, density: %.2f %%\n\n" RESET_COLOR, n, density);

    //! Create a random symmetric matrix with given size
    //! and density. Diagonial row consists of zeros
    int *X = malloc(n*n * sizeof(int));
    srand(time(0)); 
    for(int i=0; i<n; i++)
    {
        for(int j=i; j<n; j++)
        {
            if(i == j)
                X[n*i + j] = 1;
            else
            {
                double bin = (double)rand() / RAND_MAX;
                if( bin <= 0.01*density ) 
                    X[n*i + j] = 1;
                else
                    X[n*i + j] = 0;
            }
        }

        for(int j=0; j<i; j++)
            X[n*i + j] = X[n*j + i];
    }

    //! ***** Print the input matrix if n<=50 *****
    if(n<=50)
    {
        printf(YELLOW "=== Input Matrix ===\n" RESET_COLOR);
        print_array_2d(X, n);
    }

    //! ***** UNCOMMENT to Write input matrix to a file
    FILE *fp1;
    fp1 = fopen("matrices/input.csv", "w");  //create a file
    if (fp1 == NULL)
    {
        printf("Error while opening the file.\n");
        return 0;
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            fprintf(fp1, "%d", X[n*i + j]);
            if(j < n-1)
                fprintf(fp1, ",");
        }
        fprintf(fp1,"\n");
    }
    fclose(fp1);
    
    //! Calculate input bandwidth
    int bandwidth_inp = calc_bandwidth(X, n);
    printf(YELLOW "Input bandwidth: " RESET_COLOR "%d\n\n", bandwidth_inp);

    //! ========= START POINT =========
	struct timeval startwtime, endwtime;
	double p_time;
    gettimeofday (&startwtime, NULL);

    //! Implement RCM Algorithm
    int *permutation = malloc(n * sizeof(int));
    permutation = rcm(X, n);

    //! ========= END POINT =========
    gettimeofday (&endwtime, NULL);
    p_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
  		      + endwtime.tv_sec - startwtime.tv_sec);
    
    //! ***** UNCOMMENT to print the permutation array
    // printf(GREEN "Permutation: " RESET_COLOR);
    // print_array(permutation, n);

    //! Store permutation array depending on indices
    int *changes = malloc(n * sizeof(int));
    for(int i=0; i<n; i++)
        changes[permutation[i]] = i;

    //! Create the input graph according to input matrix
    Graph* inp_graph = createGraph(n);
    for(int i=0; i<n; i++)
        for(int j=i+1; j<n; j++)
            if( X[n*i + j] )
                addEdge(inp_graph, i, j);

    //! ***** UNCOMMENT to print the input graph
    // printf(YELLOW "--- Input Graph ---\n" RESET_COLOR);
    // printGraph(inp_graph);

    //! Create the output graph
    Graph* out_graph = createGraph(n);
    for(int i=0; i<n; i++)
    {
        struct node* temp = inp_graph->adjLists[permutation[i]];
        while (temp) 
        {
            addEdge(out_graph, i, changes[temp->vertex]);
            temp = temp->next;
        }
    }

    //! Create output matrix and initialize it with zeros
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            if(i == j)
                X[n*i + j] = 1;
            else
                X[n*i + j] = 0;
    
    //! Build the output matrix according to output graph
    for(int v=0; v < out_graph->numVertices; v++) 
    {
        struct node* temp = out_graph->adjLists[v];
        while (temp) {
            X[n*v + temp->vertex] = 1;
            temp = temp->next;
        }
    }

    //! ***** Print the output matrix if n<=50
    if(n<=50)
    {
        printf(YELLOW "=== Output Matrix ===\n" RESET_COLOR);
        print_array_2d(X, n);
    }

    //! Calculate output bandwidth
    int bandwidth_out = calc_bandwidth(X, n);
    printf(YELLOW "Output bandwidth: " RESET_COLOR "%d\n\n", bandwidth_out);

    //! ***** Print time elapsed 
    printf("Time elapsed: " RED "%f sec\n" RESET_COLOR, p_time);

    //! ***** UNCOMMENT to Write output matrix to a file
    FILE *fp2;
    fp2 = fopen("matrices/output.csv", "w");//create a file
    if (fp2 == NULL)
    {
        printf("Error while opening the file.\n");
        return 0;
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            fprintf(fp2, "%d", X[n*i + j]);
            if(j < n-1)
                fprintf(fp2, ",");
        }
        fprintf(fp2,"\n");
    }
    fclose(fp2);

    //! ***** UNCOMMENT to print the output graph
    // printf(YELLOW "--- Output Graph ---\n" RESET_COLOR);
    // printGraph(out_graph);

    //! Free allocated memory
    free(X);
    free(permutation);
    free(changes);

    return 0;
}