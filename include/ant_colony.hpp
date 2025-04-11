#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <random>

class AntColony : public Metaheuristic {
public:
    // Default constructor
    AntColony(const Problem& problem);
    
    // Parameterized constructor
    AntColony(const Problem& problem, int numAnts, int maxIterations, 
              double evaporationRate, double alpha, double beta);
    
    Solution solve() override;
    
private:
    struct Ant {
        std::vector<int> permutation;
        int makespan;
    };
    
    // Parameters
    int numAnts;
    int maxIterations;
    double evaporationRate;
    double alpha;  // Pheromone importance
    double beta;   // Heuristic importance
    
    // Components
    void initializePheromones();
    void constructSolutions();
    void updatePheromones();
    double calculateHeuristic(int i, int j) const;
    
    // Pheromone matrix
    std::vector<std::vector<double>> pheromones;
    std::vector<Ant> ants;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
}; 