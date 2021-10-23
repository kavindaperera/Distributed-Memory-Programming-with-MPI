#include <stdio.h>
#include <string.h> /* For strlen               */
#include <mpi.h>    /* For MPI function, etc    */

const int MAX_STRING = 100;

int main(int argc, char **argv) {  
    char        greeting[MAX_STRING];
    int         comm_sz; /* No of processes */
    int         my_rank; /* my process rank */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank != 0) {
        sprintf(greeting, "Greeting from process %d of %d!", my_rank, comm_sz);
        MPI_Send(
            greeting,               /* msg_buf_p - in */
            strlen(greeting) + 1,   /* msg_size - in */ 
            MPI_CHAR,               /* msg_type - in */ 
            0,                      /* dest - in */  
            0,                      /* tag - in */        
            MPI_COMM_WORLD);        /* communicator - in */  

    } else {
        printf("Greetings from process %d of %d!\n", my_rank, comm_sz);

        for (int q = 1; q < comm_sz; q++)
        {
            MPI_Recv(
                greeting,           /* msg_buf_p - out */ 
                MAX_STRING,         /* buf_size - in */ 
                MPI_CHAR,           /* buf_type - in */
                MPI_ANY_SOURCE,     /* source - in */
                MPI_ANY_TAG,        /* tag - in */
                MPI_COMM_WORLD,     /* communicator - in */         
                MPI_STATUS_IGNORE); /* status_p - out */

            printf("%s\n", greeting);

        }
    }
    
    MPI_Finalize();
    return 0;
}

// mpicc mpi_hello.c -o mpi_hello

// mpirun --oversubscribe -np <no of processes> mpi_hello

// mpiexec -n <no of processes> ./mpi_hello

// echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope