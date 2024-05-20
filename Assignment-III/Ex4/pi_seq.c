#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SEED 921
#define NUM_ITER 1000000000

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, numRanks;
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(SEED * rank);
    int localCount = 0, totalCount;
    int iterPerProcess = NUM_ITER / numRanks;
    double x, y, z;

	double start = MPI_Wtime();
    for (int iter = 0; iter < iterPerProcess; iter++) {
        x = (double)rand() / (double)RAND_MAX;
        y = (double)rand() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        if (z <= 1.0) {
            localCount++;
        }
    }
    double end = MPI_Wtime();

    MPI_Reduce(&localCount, &totalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = ((double)totalCount / (double)NUM_ITER) * 4.0;
        printf("Estimated Pi: %f\n", pi);
        printf("Time taken: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
