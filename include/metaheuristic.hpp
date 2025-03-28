#pragma once

#include <memory>
#include <chrono>
#include "solution.hpp"

class Metaheuristic {
public:
    Metaheuristic(const Problem& problem);
    virtual ~Metaheuristic() = default;
    
    // Main solving method to be implemented by derived classes
    virtual Solution solve() = 0;
    
    // Common methods for all metaheuristics
    Solution getBestSolution() const { return bestSolution; }
    double getExecutionTime() const { return executionTime; }
    
protected:
    const Problem& problem;
    Solution bestSolution;
    double executionTime;
    
    // Helper methods
    void startTimer();
    void stopTimer();
    
private:
    std::chrono::high_resolution_clock::time_point startTime;
}; 