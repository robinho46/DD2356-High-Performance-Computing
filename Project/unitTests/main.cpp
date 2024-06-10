#include <iostream>
#include <vector>
#include <cmath>
#include "simulation.h"

int main() {
    double dx = boxsize / N;
    double dt = (std::sqrt(2)/2) * dx / c;
    double fac = dt*dt * c*c / (dx*dx);

    std::vector<double> xlin(N);
    std::vector<std::vector<double>> U(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<bool>> mask(N, std::vector<bool>(N, false));
    std::vector<std::vector<double>> Uprev = U;
    std::vector<std::vector<double>> Unew(N, std::vector<double>(N, 0.0));

    initializeGrid(U, mask, xlin);

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

