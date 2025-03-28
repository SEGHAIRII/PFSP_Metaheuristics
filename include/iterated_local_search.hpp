#pragma once

#include "metaheuristic.hpp"

class IteratedLocalSearch : public Metaheuristic {
public:
    IteratedLocalSearch(const Problem& problem);
    Solution solve() override;
    
private:
    // Components
    void perturbation(Solution& current);
    void localSearch(Solution& current);
    
    // Parameters
    static constexpr int MAX_ITERATIONS = 100;
    static constexpr int PERTURBATION_STRENGTH = 3;
}; 