#include <stdio.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>

double pi_calc(long int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); //get total number of processes

    srand(time(NULL) + rank); //seed random number generator differently for each process

    long int local_n = n / size; //average number of points per process
    if (rank < n % size) { //handle the case when n is not divisible by the number of processes
        local_n++;
    }

    //generate random points and count how many fall within the unit circle
    long int local_count = 0;
    for (long int i = 0; i < local_n; i++) {
        double x = (double)rand() / RAND_MAX; //generate random x in [0, 1] 
        double y = (double)rand() / RAND_MAX; //generate random y in [0, 1]
        if (x * x + y * y <= 1.0) {  //check if point is inside unit circle
            local_count++;
        }
    }

    //sum up the counts from all processes
    long int global_count;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi_estimate = 0.0;
    if (rank == 0) {
        pi_estimate = 4.0 * global_count / n;
    }

    return pi_estimate;
}
