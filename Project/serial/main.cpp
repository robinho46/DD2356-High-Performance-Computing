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

// Constants
const int N = 256; /**< Grid size */
const double boxsize = 1.0; /**< Size of the computational domain */
const double c = 1.0; /**< Speed of propagation */
const double tEnd = 2.0; /**< End time of simulation */

/**
 * @brief Initializes the grid and sets boundary conditions.
 * 
 * This function initializes the grid and sets boundary conditions based on the mask.
 * @param U Grid values to initialize
 * @param mask Boundary mask indicating boundary points
 * @param xlin Array of spatial coordinates
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

    for (int i = N/4; i < 9*N/32; ++i) {
        for (int j = 0; j < N-1; ++j) {
            mask[i][j] = true;
        }
    }

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
 * @brief Applies boundary conditions to the grid.
 *
 * This function applies boundary conditions to the grid based on the mask and time parameter.
 * @param U Grid values to apply boundary conditions
 * @param mask Boundary mask indicating boundary points
 * @param t Time parameter
 * @param xlin Array of spatial coordinates
 */
void applyBoundaryConditions(std::vector<std::vector<double>>& U, const std::vector<std::vector<bool>>& mask, double t, const std::vector<double>& xlin) {
    for (int i = 0; i < N; ++i) {
        if (mask[i][0] || mask[i][N-1] || mask[0][i] || mask[N-1][i]) {
            U[i][0] = U[i][N-1] = U[0][i] = U[N-1][i] = 0.0;
        }
    }

    for (int i = 0; i < N; ++i) {
        U[0][i] = std::sin(20.0 * M_PI * t) * std::pow(std::sin(M_PI * xlin[i]), 2);
    }
}

/**
 * @brief Updates the Laplacian of the grid.
 *
 * This function calculates the Laplacian of the grid and updates it using finite difference method.
 * @param U Current grid values
 * @param Unew Updated grid values after Laplacian calculation
 * @param mask Boundary mask indicating boundary points
 * @param fac Scaling factor
 */
void updateLaplacian(const std::vector<std::vector<double>>& U, std::vector<std::vector<double>>& Unew, const std::vector<std::vector<bool>>& mask, double fac) {
    for (int i = 1; i < N-1; ++i) {
        for (int j = 1; j < N-1; ++j) {
            if (!mask[i][j]) {
                double ULX = U[i-1][j];
                double URX = U[i+1][j];
                double ULY = U[i][j-1];
                double URY = U[i][j+1];
                double laplacian = (ULX + URX + ULY + URY - 4.0 * U[i][j]);
                Unew[i][j] = 2.0 * U[i][j] - U[i][j] + fac * laplacian;
            }
        }
    }
}


int main() {
    double dx = boxsize / N;
    double dt = (std::sqrt(2) / 2) * dx / c;
    double fac = dt * dt * c * c / (dx * dx);

    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(N, std::vector<bool>(N, false));

    initializeGrid(U, mask, xlin);

    std::vector<std::vector<double>> Uprev = U;
    std::vector<std::vector<double>> Unew(N, std::vector<double>(N, 0.0));

    double t = 0.0;

    while (t < tEnd) {
        updateLaplacian(U, Unew, mask, fac);

        Uprev = U;
        U = Unew;

        applyBoundaryConditions(U, mask, t, xlin);

        t += dt;
        std::cout << t << std::endl;
    }
    return 0;
}
