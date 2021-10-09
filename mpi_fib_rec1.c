#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* We'll be using MPI routines, definitions, etc. */
#include <mpi.h>

#define BOUND 20

/* Fibonacci Function */
int fib(int n);

int main(int argc, char *argv[])
{
    int my_rank, comm_sz, local_n;
    int local_start, local_end;
    double total_sum = 0, local_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("start process %d of %d processes\n", my_rank, comm_sz);

    local_n = BOUND / comm_sz; /* same for all processes  */

    int fib_index;

    for (int i = 0; i < local_n; i++)
    {
        fib_index = comm_sz*i + my_rank + 1;
        local_sum += pow(fib(fib_index), -1);
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    // (const void *sendbuf, void *recvbuf, int count,
    //                MPI_Datatype datatype, MPI_Op op, int root,
    //                MPI_Comm comm)


    /* Print the result */
    if (my_rank == 0)
    {
        printf("Sum of Fibonacci Reciprocals: %f\n", total_sum);
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;

} /*  main  */

/*------------------------------------------------------------------
 * Function:    Fibonacci
 * Purpose:     Returns n th fibonacci number
 * Input args:  x
 */
int fib(int n)
{
    if (n == 1)
        return 1;

    if (n == 2)
        return 2;

    return fib(n-1) + fib(n-2);     
} /* fib */