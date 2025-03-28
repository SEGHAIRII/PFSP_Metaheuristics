#include "ant_colony.hpp"
#include <cmath>
#include <algorithm>

AntColony::AntColony(const Problem& problem) 
    : Metaheuristic(problem), gen(rd()) {
    ants.resize(NUM_ANTS);
    for (auto& ant : ants) {
        ant.permutation.resize(problem.getNumJobs());
    }
}

Solution AntColony::solve() {
    startTimer();
    
    initializePheromones();
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        constructSolutions();
        updatePheromones();
        
        // Update best solution
        for (const auto& ant : ants) {
            if (ant.makespan < bestSolution.getMakespan()) {
                bestSolution.setPermutation(ant.permutation);
            }
        }
        
        iterations++;
    }
    
    stopTimer();
    return bestSolution;
}

void AntColony::initializePheromones() {
    int n = problem.getNumJobs();
    pheromones.resize(n, std::vector<double>(n, 1.0));
}

void AntColony::constructSolutions() {
    int n = problem.getNumJobs();
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (auto& ant : ants) {
        std::vector<bool> used(n, false);
        ant.permutation[0] = std::uniform_int_distribution<>(0, n-1)(gen);
        used[ant.permutation[0]] = true;
        
        // Construct permutation
        for (int i = 1; i < n; ++i) {
            std::vector<double> probabilities(n, 0.0);
            double sum = 0.0;
            
            // Calculate probabilities
            for (int j = 0; j < n; ++j) {
                if (!used[j]) {
                    probabilities[j] = std::pow(pheromones[ant.permutation[i-1]][j], ALPHA) *
                                     std::pow(calculateHeuristic(ant.permutation[i-1], j), BETA);
                    sum += probabilities[j];
                }
            }
            
            // Normalize probabilities
            for (double& prob : probabilities) {
                prob /= sum;
            }
            
            // Roulette wheel selection
            double r = dis(gen);
            double cumsum = 0.0;
            int selected = -1;
            
            for (int j = 0; j < n; ++j) {
                if (!used[j]) {
                    cumsum += probabilities[j];
                    if (r <= cumsum) {
                        selected = j;
                        break;
                    }
                }
            }
            
            if (selected == -1) {
                // Fallback to random selection
                std::vector<int> available;
                for (int j = 0; j < n; ++j) {
                    if (!used[j]) available.push_back(j);
                }
                selected = available[std::uniform_int_distribution<>(0, available.size()-1)(gen)];
            }
            
            ant.permutation[i] = selected;
            used[selected] = true;
        }
        
        // Evaluate solution
        Solution temp(problem);
        temp.setPermutation(ant.permutation);
        ant.makespan = temp.getMakespan();
    }
}

void AntColony::updatePheromones() {
    int n = problem.getNumJobs();
    
    // Evaporate pheromones
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromones[i][j] *= (1.0 - EVAPORATION_RATE);
        }
    }
    
    // Add new pheromones
    for (const auto& ant : ants) {
        double contribution = 1.0 / ant.makespan;
        for (int i = 0; i < n - 1; ++i) {
            pheromones[ant.permutation[i]][ant.permutation[i+1]] += contribution;
            pheromones[ant.permutation[i+1]][ant.permutation[i]] += contribution;
        }
    }
}

double AntColony::calculateHeuristic(int i, int j) const {
    // Calculate average processing time difference
    double sum = 0.0;
    for (int m = 0; m < problem.getNumMachines(); ++m) {
        sum += problem.getProcessingTimes()[m][j] - problem.getProcessingTimes()[m][i];
    }
    return 1.0 / (1.0 + std::abs(sum / problem.getNumMachines()));
} 