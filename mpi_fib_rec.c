#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>


int fib(int n);

int main(void){

    int my_rank, comm_sz;
    int local_n, n = 40;
    int fib[n];
    double total_sum = 0, local_sum = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    local_n = n /comm_sz;
    int local_fib[local_n];

    if (my_rank == 0)
    {
        fib[0] = 1;
        fib[1] = 1;
        for (int i = 2; i < n; i++)
        {
            fib[i] = fib[i-1] + fib[i-2];
        }
        
    }
    
    MPI_Scatter(&fib, local_n, MPI_INT, &local_fib, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
    {
        local_sum += pow(local_fib[i], -1);
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (my_rank == 0) {
        printf("With n = %d , our estimate\n", n);
        printf("is %.15e\n", total_sum);
    }  


    MPI_Finalize();
    return 0;
}   /*  main  */

int fib(int n){
    if (n==1)
        return 1;
    if (n==2)
        return 1;
    return fib(n-1) + fib(n-2);        
}   /* fib */