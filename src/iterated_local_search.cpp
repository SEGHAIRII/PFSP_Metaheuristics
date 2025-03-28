#include "iterated_local_search.hpp"
#include <random>
#include <algorithm>
#include <numeric>

IteratedLocalSearch::IteratedLocalSearch(const Problem& problem) : Metaheuristic(problem) {}

Solution IteratedLocalSearch::solve() {
    startTimer();
    
    // Initialize with a random solution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> initialPerm(problem.getNumJobs());
    std::iota(initialPerm.begin(), initialPerm.end(), 0);
    std::shuffle(initialPerm.begin(), initialPerm.end(), gen);
    
    Solution current(problem);
    current.setPermutation(initialPerm);
    bestSolution = current;
    
    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        Solution previous = current;
        perturbation(current);
        localSearch(current);
        
        // Accept if better or with probability
        if (current.getMakespan() < bestSolution.getMakespan()) {
            bestSolution = current;
        } else {
            current = previous;
        }
        
        iterations++;
    }
    
    stopTimer();
    return bestSolution;
}

void IteratedLocalSearch::perturbation(Solution& current) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, problem.getNumJobs() - 1);
    
    // Perform random swaps
    for (int i = 0; i < PERTURBATION_STRENGTH; ++i) {
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        if (pos1 != pos2) {
            current.swap(pos1, pos2);
        }
    }
}

void IteratedLocalSearch::localSearch(Solution& current) {
    bool improved;
    do {
        improved = false;
        int n = problem.getNumJobs();
        
        // Try swap neighborhood
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Solution temp = current;
                temp.swap(i, j);
                
                if (temp.getMakespan() < current.getMakespan()) {
                    current = temp;
                    improved = true;
                }
            }
        }
    } while (improved);
} 