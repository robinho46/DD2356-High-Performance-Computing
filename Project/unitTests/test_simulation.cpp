#include <iostream>
#include <vector>
#include "simulation.h"

void test_initializeGrid() {
    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(N, std::vector<bool>(N, false));

    initializeGrid(U, mask, xlin);

    bool passed = true;
    for (int i = 0; i < N; ++i) {
        if (!mask[0][i] || !mask[N-1][i] || !mask[i][0] || !mask[i][N-1]) {
            passed = false;
            break;
        }
    }

    std::cout << "test_initializeGrid: " << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_applyBoundaryConditions() {
    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(N, std::vector<bool>(N, false));

    initializeGrid(U, mask, xlin);

    double t = 0.0;
    applyBoundaryConditions(U, mask, t, xlin);

    bool passed = true;
    for (int i = 0; i < N; ++i) {
        if (U[i][0] != 0.0 || U[i][N-1] != 0.0 || U[0][i] != 0.0 || U[N-1][i] != 0.0) {
            passed = false;
            break;
        }
    }

    std::cout << "test_applyBoundaryConditions: " << (passed ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    test_initializeGrid();
    test_applyBoundaryConditions();
    return 0;
}

