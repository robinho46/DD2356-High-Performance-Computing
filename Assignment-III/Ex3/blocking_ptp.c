#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int rank, size, i, provided;

  // number of cells (global)
  int nxc = 128;         // make sure nxc is divisible by size
  double L = 2 * 3.1415; // Length of the domain

  MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells
  int nxn_loc = nxc / size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
  double L_loc = L / ((double)size);
  double dx = L / ((double)nxc);

  // define out function
  double *f = calloc(nxn_loc, sizeof(double));    // allocate and fill with z
  double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

  for (i = 1; i < (nxn_loc - 1); i++)
    f[i] = sin(L_loc * rank + (i - 1) * dx);

  // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
  // communicate ghost cells

  int Prevoius_rank = rank - 1;
  int Next_rank = rank + 1;
  
  if (rank == 0)
    prevRank = size - 1;
  
  else if (rank == size - 1)
    nextRank = 0;
  
  // Send to rank below
  MPI_Send(&f[1], 1, MPI_DOUBLE, Prevoius_rank, 0, MPI_COMM_WORLD);
  // Receive from rank above
  MPI_Recv(&f[nxn_loc - 1], 1, MPI_DOUBLE, Next_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // Send to rank above
  MPI_Send(&f[nxn_loc - 2], 1, MPI_DOUBLE, Next_rank, 1, MPI_COMM_WORLD);
  // Receive from rank below
  MPI_Recv(&f[0], 1, MPI_DOUBLE, Prevoius_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


  // calculate first order derivative using central difference
  // here we need to correct value of the ghost cells!
  for (i = 1; i < (nxn_loc - 1); i++)
    dfdx[i] = (f[i + 1] - f[i - 1]) / (2 * dx);

  if (rank == 2)
  {
    for (int i = 1; i < 6; i++)
    {
      double diff = fabs(cos(L_loc * rank + (i - 1) * dx) - dfdx[i]);
      printf("%f\n", diff);
    }

    printf(".\n");
    printf(".\n");
    printf(".\n");
    printf(".\n");

    for (int i = nxn_loc - 6; i <= (nxn_loc - 1); i++)
    {
      double diff = fabs(cos(L_loc * rank + (i - 1) * dx) - dfdx[i]);
      printf("%f\n", diff);
    }
  }
  MPI_Finalize();
}