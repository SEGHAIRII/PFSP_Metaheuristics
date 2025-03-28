#include "simulated_annealing.hpp"
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

SimulatedAnnealing::SimulatedAnnealing(const Problem& problem) 
    : Metaheuristic(problem), gen(rd()), temperature(INITIAL_TEMPERATURE), iterations(0) {}

Solution SimulatedAnnealing::solve() {
    startTimer();
    
    // Initialize with a random solution
    std::vector<int> initialPerm(problem.getNumJobs());
    std::iota(initialPerm.begin(), initialPerm.end(), 0);
    std::shuffle(initialPerm.begin(), initialPerm.end(), gen);
    
    Solution current(problem);
    current.setPermutation(initialPerm);
    bestSolution = current;
    
    iterations = 0;
    temperature = INITIAL_TEMPERATURE;
    
    while (iterations < MAX_ITERATIONS) {
        // Generate random neighbor
        std::uniform_int_distribution<> dis(0, problem.getNumJobs() - 1);
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        if (pos1 != pos2) {
            Solution neighbor = current;
            neighbor.swap(pos1, pos2);
            
            // Accept if better or with probability
            if (neighbor.getMakespan() < current.getMakespan() || 
                acceptWorseSolution(current.getMakespan(), neighbor.getMakespan())) {
                current = neighbor;
                
                if (current.getMakespan() < bestSolution.getMakespan()) {
                    bestSolution = current;
                }
            }
        }
        
        updateTemperature();
        iterations++;
    }
    
    stopTimer();
    return bestSolution;
}

bool SimulatedAnnealing::acceptWorseSolution(int currentMakespan, int newMakespan) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double delta = newMakespan - currentMakespan;
    double probability = std::exp(-delta / temperature);
    return dis(gen) < probability;
}

void SimulatedAnnealing::updateTemperature() {
    temperature *= COOLING_RATE;
} 