#include "simulated_annealing.hpp"
#include "constructive.hpp"
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

SimulatedAnnealing::SimulatedAnnealing(const Problem& problem) 
    : Metaheuristic(problem), 
      maxIterations(1000),
      initialTemperature(1000.0),
      coolingRate(0.95),
      temperature(initialTemperature), 
      iterations(0), 
      gen(rd()) {}

SimulatedAnnealing::SimulatedAnnealing(const Problem& problem, int maxIterations, 
                                      double initialTemperature, double coolingRate) 
    : Metaheuristic(problem), 
      maxIterations(maxIterations),
      initialTemperature(initialTemperature),
      coolingRate(coolingRate),
      temperature(initialTemperature), 
      iterations(0), 
      gen(rd()) {}

      Solution SimulatedAnnealing::solve() {
        Constructive neh = Constructive(problem);
        Solution initial_solution = neh.solve();
        startTimer();
    
        bestSolution = initial_solution;
    
        Solution current = bestSolution;
        iterations = 0;
        temperature = initialTemperature;
    
        while (iterations < maxIterations) {
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
    temperature *= coolingRate;
} 