/**
 * @file simulation.cpp
 * @brief Implementation of functions related to simulation and grid operations.
 * 
 * This source file contains implementations of functions declared in simulation.h.
 * It includes functions for initializing the grid, applying boundary conditions,
 * and updating the Laplacian to simulate the behavior of a partial differential
 * equation using finite difference methods.
 */

#include "simulation.h"
#include <cmath>
#include <iostream>

const int N = 256;
const double boxsize = 1.0;
const double c = 1.0;
const double tEnd = 2.0;

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

