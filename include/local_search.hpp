#pragma once

#include "metaheuristic.hpp"

class LocalSearch : public Metaheuristic {
public:
    // Default constructor
    LocalSearch(const Problem& problem);
    
    // Parameterized constructor
    LocalSearch(const Problem& problem, int maxIterations);
    
    Solution solve() override;
    
private:
    // Different neighborhood structures
    bool swapNeighborhood();
    bool insertNeighborhood();
    bool reverseNeighborhood();
    
    // Parameters
    int maxIterations;
}; 