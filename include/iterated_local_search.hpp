#pragma once

#include "metaheuristic.hpp"

class IteratedLocalSearch : public Metaheuristic {
public:
    // Default constructor
    IteratedLocalSearch(const Problem& problem);
    
    // Parameterized constructor
    IteratedLocalSearch(const Problem& problem, int maxIterations, int perturbationStrength);
    
    Solution solve() override;
    
private:
    // Components
    void perturbation(Solution& current);
    void localSearch(Solution& current);
    
    // Parameters
    int maxIterations;
    int perturbationStrength;
}; 