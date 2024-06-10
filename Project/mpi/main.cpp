/**
 * @file main.cpp
 * @brief Solves a partial differential equation using a finite difference method.
 * 
 * This program solves a partial differential equation using a finite difference method.
 * It initializes a grid, applies boundary conditions, and updates the Laplacian iteratively.
 * The solution is obtained for a given time interval.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

// Constants
const int N = 256;
const double boxsize = 1.0;
const double c = 1.0;
const double tEnd = 2.0;

/**
 * @brief Initializes the grid and boundary conditions.
 * 
 * @param U Vector of vectors representing the grid values.
 * @param mask Vector of vectors representing the grid mask.
 * @param xlin Vector storing the spatial coordinates.
 * @param local_N Number of rows assigned to the current process.
 */
void initializeGrid(std::vector<std::vector<double>>& U, std::vector<std::vector<bool>>& mask, std::vector<double>& xlin, int local_N) {
    double dx = boxsize / N;
    for (int i = 0; i < N; ++i) {
        xlin[i] = 0.5 * dx + i * dx;
    }

    // Initialize mask based on process's assigned domain
    for (int i = 0; i < local_N; ++i) {
        mask[i][0] = true;
        mask[i][N-1] = true;
    }
}

/**
 * @brief Calculates the Laplacian of the grid.
 * 
 * @param U Current grid values.
 * @param Uprev Previous grid values.
 * @param mask Grid mask.
 * @param Unew New grid values after Laplacian calculation.
 * @param fac Factor used in the numerical approximation.
 * @param local_N Number of rows assigned to the current process.
 */
void calculateLaplacian(std::vector<std::vector<double>>& U, const std::vector<std::vector<double>>& Uprev,
                        const std::vector<std::vector<bool>>& mask, std::vector<std::vector<double>>& Unew, double fac, int local_N) {
    for (int i = 1; i < local_N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            if (!mask[i][j]) {
                double ULX = U[i][j - 1];
                double URX = U[i][j + 1];
                double ULY = U[i - 1][j];
                double URY = U[i + 1][j];
                double laplacian = (ULX + URX + ULY + URY - 4.0 * U[i][j]);
                Unew[i][j] = 2.0 * U[i][j] - Uprev[i][j] + fac * laplacian;
            }
        }
    }
}

/**
 * @brief Applies boundary conditions to the grid.
 * 
 * @param U Grid values.
 * @param mask Grid mask.
 * @param xlin Vector storing the spatial coordinates.
 * @param t Current time.
 * @param local_N Number of rows assigned to the current process.
 */
void applyBoundaryConditions(std::vector<std::vector<double>>& U, const std::vector<std::vector<bool>>& mask, std::vector<double>& xlin, double t, int local_N) {
    for (int i = 0; i < local_N; ++i) {
        U[i][0] = U[i][N - 1] = 0.0;
    }

    for (int i = 0; i < local_N; ++i) {
        U[i][0] = std::sin(20.0 * M_PI * t) * std::pow(std::sin(M_PI * xlin[i]), 2);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Start the timer
    double start_time = MPI_Wtime();

    // Determine the domain size for each process
    int local_N = N / size;
    int start_row = rank * local_N;
    int end_row = (rank + 1) * local_N;

    // Simulation parameters
    double dx = boxsize / N;
    double dt = (std::sqrt(2)/2) * dx / c;
    double fac = dt*dt * c*c / (dx*dx);

    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(local_N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(local_N, std::vector<bool>(N, false));
    std::vector<std::vector<double>> Uprev(local_N, std::vector<double>(N, 0.0));
    std::vector<std::vector<double>> Unew(local_N, std::vector<double>(N, 0.0));

    initializeGrid(U, mask, xlin, local_N);

    double t = 0.0;

    while (t < tEnd) {
        // calculate laplacian
        calculateLaplacian(U, Uprev, mask, Unew, fac, local_N);

        Uprev = U;
        U = Unew;

        // apply boundary conditions (Dirichlet/inflow)
        applyBoundaryConditions(U, mask, xlin, t, local_N);

        t += dt;
    }

    // Stop the timer and calculate the elapsed time
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    // Gather the elapsed times from all processes to the root process
    std::vector<double> all_times;
    if (rank == 0) {
        all_times.resize(size);
    }
    MPI_Gather(&elapsed_time, 1, MPI_DOUBLE, all_times.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print the elapsed times for all processes on the root process
    if (rank == 0) {
        double totalExecutionTime = 0.0;
        for (int i = 0; i < size; ++i) {
            std::cout << "Process " << i << " Execution Time: " << all_times[i] << " seconds" << std::endl;
            totalExecutionTime += all_times[i];
        }
        std::cout << "Total Execution Time: " << totalExecutionTime << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

