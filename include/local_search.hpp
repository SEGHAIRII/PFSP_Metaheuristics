#pragma once

#include "metaheuristic.hpp"

class LocalSearch : public Metaheuristic {
public:
    LocalSearch(const Problem& problem);
    Solution solve() override;
    
private:
    // Different neighborhood structures
    bool swapNeighborhood();
    bool insertNeighborhood();
    bool reverseNeighborhood();
    
    // Parameters
    static constexpr int MAX_ITERATIONS = 1000;
}; 