#include "tabu_search.hpp"
#include "constructive.hpp"
#include <random>
#include <algorithm>
#include <numeric>

TabuSearch::TabuSearch(const Problem& problem) 
    : Metaheuristic(problem), maxIterations(1000), tabuListSize(10) {}

TabuSearch::TabuSearch(const Problem& problem, int maxIterations, int tabuListSize) 
    : Metaheuristic(problem), maxIterations(maxIterations), tabuListSize(tabuListSize) {}

    Solution TabuSearch::solve() {
        Constructive neh = Constructive(problem);
        Solution initial_solution = neh.solve();
        startTimer();
    
        bestSolution = initial_solution;
    
        Solution current = bestSolution;
        int iterations = 0;
    
        while (iterations < maxIterations) {
            auto [i, j] = findBestNeighbor();
            if (i != -1 && j != -1) {
                current.swap(i, j);
                updateTabuList(i, j);
    
                if (current.getMakespan() < bestSolution.getMakespan()) {
                    bestSolution = current;
                }
            }
            iterations++;
        }
    
        stopTimer();
        return bestSolution;
    }

bool TabuSearch::isTabu(int i, int j) const {
    auto pair1 = std::make_pair(i, j);
    auto pair2 = std::make_pair(j, i);
    return std::find(tabuList.begin(), tabuList.end(), pair1) != tabuList.end() ||
           std::find(tabuList.begin(), tabuList.end(), pair2) != tabuList.end();
}

void TabuSearch::updateTabuList(int i, int j) {
    tabuList.push_back({i, j});
    if (tabuList.size() > tabuListSize) {
        tabuList.pop_front();
    }
}

std::pair<int, int> TabuSearch::findBestNeighbor() {
    int bestI = -1, bestJ = -1;
    int bestMakespan = std::numeric_limits<int>::max();
    int n = problem.getNumJobs();
    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (!isTabu(i, j)) {
                Solution neighbor = bestSolution;
                neighbor.swap(i, j);
                
                if (neighbor.getMakespan() < bestMakespan) {
                    bestMakespan = neighbor.getMakespan();
                    bestI = i;
                    bestJ = j;
                }
            }
        }
    }
    
    return {bestI, bestJ};
} 