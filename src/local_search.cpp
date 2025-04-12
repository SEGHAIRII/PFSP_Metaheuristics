#include "local_search.hpp"
#include "constructive.hpp"
#include <random>
#include <algorithm>
#include <numeric>

LocalSearch::LocalSearch(const Problem& problem) 
    : Metaheuristic(problem), maxIterations(1000) {}

LocalSearch::LocalSearch(const Problem& problem, int maxIterations) 
    : Metaheuristic(problem), maxIterations(maxIterations) {}

Solution LocalSearch::solve() {
    Constructive neh = Constructive(problem);
        Solution initial_solution = neh.solve();
        startTimer();
    
        bestSolution = initial_solution;
    
        Solution current = bestSolution;
        bool improved;
        int iterations = 0;
    
        do {
            improved = false;
    
            // Try different neighborhoods
            if (swapNeighborhood()) improved = true;
            if (insertNeighborhood()) improved = true;
            if (reverseNeighborhood()) improved = true;
    
            iterations++;
        } while (improved && iterations < maxIterations);
    
        stopTimer();
        return bestSolution;
}

bool LocalSearch::swapNeighborhood() {
    bool improved = false;
    int n = problem.getNumJobs();
    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Solution current = bestSolution;
            current.swap(i, j);
            
            if (current.getMakespan() < bestSolution.getMakespan()) {
                bestSolution = current;
                improved = true;
            }
        }
    }
    
    return improved;
}

bool LocalSearch::insertNeighborhood() {
    bool improved = false;
    int n = problem.getNumJobs();
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                Solution current = bestSolution;
                current.insert(i, j);
                
                if (current.getMakespan() < bestSolution.getMakespan()) {
                    bestSolution = current;
                    improved = true;
                }
            }
        }
    }
    
    return improved;
}

bool LocalSearch::reverseNeighborhood() {
    bool improved = false;
    int n = problem.getNumJobs();
    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Solution current = bestSolution;
            current.reverse(i, j);
            
            if (current.getMakespan() < bestSolution.getMakespan()) {
                bestSolution = current;
                improved = true;
            }
        }
    }
    
    return improved;
} 