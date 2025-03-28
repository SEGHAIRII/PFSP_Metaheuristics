#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <deque>
#include <utility>

class TabuSearch : public Metaheuristic {
public:
    TabuSearch(const Problem& problem);
    Solution solve() override;
    
private:
    // Components
    bool isTabu(int i, int j) const;
    void updateTabuList(int i, int j);
    std::pair<int, int> findBestNeighbor();
    
    // Parameters
    static constexpr int MAX_ITERATIONS = 1000;
    static constexpr int TABU_LIST_SIZE = 10;
    
    // Tabu list
    std::deque<std::pair<int, int>> tabuList;
}; 