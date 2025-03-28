#pragma once

#include "metaheuristic.hpp"

class Constructive : public Metaheuristic {
public:
    Constructive(const Problem& problem);
    Solution solve() override;
    
private:
    // Different constructive methods
    std::vector<int> nehAlgorithm();
    std::vector<int> randomPermutation();
    std::vector<int> shortestProcessingTime();
}; 