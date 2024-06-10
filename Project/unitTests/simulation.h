/**
 * @file simulation.h
 * @brief Declarations of functions related to simulation and grid operations.
 * 
 * This header file contains declarations for functions used in simulating 
 * a partial differential equation using finite difference methods. It also
 * declares functions related to initializing the grid, applying boundary
 * conditions, and updating the Laplacian.
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

// Constants
extern const int N;
extern const double boxsize;
extern const double c;
extern const double tEnd;

/**
 * @brief Initializes the grid and boundary conditions.
 * 
 * @param U Vector of vectors representing the grid values.
 * @param mask Vector of vectors representing the grid mask.
 * @param xlin Vector storing the spatial coordinates.
 */
void initializeGrid(std::vector<std::vector<double>>& U, std::vector<std::vector<bool>>& mask, std::vector<double>& xlin);

/**
 * @brief Applies boundary conditions to the grid.
 * 
 * @param U Grid values.
 * @param mask Grid mask.
 * @param t Current time.
 * @param xlin Vector storing the spatial coordinates.
 */
void applyBoundaryConditions(std::vector<std::vector<double>>& U, const std::vector<std::vector<bool>>& mask, double t, const std::vector<double>& xlin);

/**
 * @brief Updates the Laplacian of the grid.
 * 
 * @param U Current grid values.
 * @param Unew New grid values after Laplacian calculation.
 * @param mask Grid mask.
 * @param fac Factor used in the numerical approximation.
 */
void updateLaplacian(const std::vector<std::vector<double>>& U, std::vector<std::vector<double>>& Unew, const std::vector<std::vector<bool>>& mask, double fac);

#endif // SIMULATION_H

