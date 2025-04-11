#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <deque>
#include <utility>

class TabuSearch : public Metaheuristic {
public:
    // Default constructor
    TabuSearch(const Problem& problem);
    
    // Parameterized constructor
    TabuSearch(const Problem& problem, int maxIterations, int tabuListSize);
    
    Solution solve() override;
    
private:
    // Components
    bool isTabu(int i, int j) const;
    void updateTabuList(int i, int j);
    std::pair<int, int> findBestNeighbor();
    
    // Parameters
    int maxIterations;
    int tabuListSize;
    
    // Tabu list
    std::deque<std::pair<int, int>> tabuList;
}; 