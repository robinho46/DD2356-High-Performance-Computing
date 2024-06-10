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
#include <limits>
#include <omp.h>
#include <chrono> // For timing

// Constants
const int N = 256; // Change grid size to 256
const double boxsize = 1.0;
const double c = 1.0;
const double tEnd = 2.0;

/**
 * @brief Initializes the grid and boundary conditions.
 * 
 * @param U Vector of vectors representing the grid values.
 * @param mask Vector of vectors representing the grid mask.
 * @param xlin Vector storing the spatial coordinates.
 */
void initializeGrid(std::vector<std::vector<double>>& U, std::vector<std::vector<bool>>& mask, std::vector<double>& xlin) {
    double dx = boxsize / N;
    for (int i = 0; i < N; ++i) {
        xlin[i] = 0.5 * dx + i * dx;
    }

    for (int i = 0; i < N; ++i) {
        mask[0][i] = true;
        mask[N-1][i] = true;
        mask[i][0] = true;
        mask[i][N-1] = true;
    }

    #pragma omp parallel for
    for (int i = N/4; i < 9*N/32; ++i) {
        for (int j = 0; j < N-1; ++j) {
            mask[i][j] = true;
        }
    }

    #pragma omp parallel for
    for (int i = 1; i < N-1; ++i) {
        for (int j = 5*N/16; j < 3*N/8; ++j) {
            mask[i][j] = false;
        }
        for (int j = 5*N/8; j < 11*N/16; ++j) {
            mask[i][j] = false;
        }
    }
}

/**
 * @brief Calculates the Laplacian of the grid.
 * 
 * @param U Current grid values.
 * @param Unew New grid values after Laplacian calculation.
 * @param mask Grid mask.
 * @param fac Factor used in the numerical approximation.
 */
void calculateLaplacian(std::vector<std::vector<double>>& U, std::vector<std::vector<double>>& Unew,
                        const std::vector<std::vector<bool>>& mask, double fac) {
    std::vector<std::vector<double>> Uprev = U;
    #pragma omp parallel for collapse(2)
    for (int i = 1; i < N-1; ++i) {
        for (int j = 1; j < N-1; ++j) {
            if (!mask[i][j]) {
                double ULX = U[i-1][j];
                double URX = U[i+1][j];
                double ULY = U[i][j-1];
                double URY = U[i][j+1];
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
 * @param t Current time.
 * @param xlin Vector storing the spatial coordinates.
 */
void applyBoundaryConditions(std::vector<std::vector<double>>& U, const std::vector<std::vector<bool>>& mask, double t,
                             const std::vector<double>& xlin) {
    for (int i = 0; i < N; ++i) {
        if (mask[i][0] || mask[i][N-1] || mask[0][i] || mask[N-1][i]) {
            U[i][0] = U[i][N-1] = U[0][i] = U[N-1][i] = 0.0;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        U[0][i] = std::sin(20.0 * M_PI * t) * std::pow(std::sin(M_PI * xlin[i]), 2);
    }
}

int main() {
    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(N, std::vector<bool>(N, false));

    initializeGrid(U, mask, xlin);

    double dx = boxsize / N;
    double dt = (std::sqrt(2)/2) * dx / c;
    double fac = dt*dt * c*c / (dx*dx);

    double t = 0.0;

    // Timing variables
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    double duration;

    // Array of thread counts
    int threads[] = {1, 32, 64, 128};

    // Run the program with different numbers of threads
    for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i) {
        omp_set_num_threads(threads[i]);

        // Start timing
        start = std::chrono::high_resolution_clock::now();

        // Main loop
        while (t < tEnd) {
            calculateLaplacian(U, U, mask, fac);
            applyBoundaryConditions(U, mask, t, xlin);
            t += dt;
        }

        // End timing
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;

        // Output execution time
        std::cout << "Threads: " << threads[i] << ", Execution time: " << duration << " seconds\n";

        // Reset time for next iteration
        t = 0.0;
    }

    return 0;
}

