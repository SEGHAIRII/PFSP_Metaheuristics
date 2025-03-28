#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <random>

class AntColony : public Metaheuristic {
public:
    AntColony(const Problem& problem);
    Solution solve() override;
    
private:
    struct Ant {
        std::vector<int> permutation;
        int makespan;
    };
    
    // Parameters
    static constexpr int NUM_ANTS = 20;
    static constexpr int MAX_ITERATIONS = 100;
    static constexpr double EVAPORATION_RATE = 0.1;
    static constexpr double ALPHA = 1.0;  // Pheromone importance
    static constexpr double BETA = 2.0;   // Heuristic importance
    
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