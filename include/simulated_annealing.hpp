#pragma once

#include "metaheuristic.hpp"
#include <random>

class SimulatedAnnealing : public Metaheuristic {
public:
    SimulatedAnnealing(const Problem& problem);
    Solution solve() override;
    
private:
    // Components
    bool acceptWorseSolution(int currentMakespan, int newMakespan);
    void updateTemperature();
    
    // Parameters
    static constexpr int MAX_ITERATIONS = 1000;
    static constexpr double INITIAL_TEMPERATURE = 1000.0;
    static constexpr double COOLING_RATE = 0.95;
    
    // State variables
    double temperature;
    int iterations;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
}; 