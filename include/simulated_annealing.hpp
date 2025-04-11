#pragma once

#include "metaheuristic.hpp"
#include <random>

class SimulatedAnnealing : public Metaheuristic {
public:
    // Default constructor
    SimulatedAnnealing(const Problem& problem);
    
    // Parameterized constructor
    SimulatedAnnealing(const Problem& problem, int maxIterations, 
                       double initialTemperature, double coolingRate);
    
    Solution solve() override;
    
private:
    // Components
    bool acceptWorseSolution(int currentMakespan, int newMakespan);
    void updateTemperature();
    
    // Parameters
    int maxIterations;
    double initialTemperature;
    double coolingRate;
    
    // State variables
    double temperature;
    int iterations;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
}; 