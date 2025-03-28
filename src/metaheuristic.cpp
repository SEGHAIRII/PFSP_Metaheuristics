#include "metaheuristic.hpp"
#include <chrono>

Metaheuristic::Metaheuristic(const Problem& problem) 
    : problem(problem), bestSolution(problem), executionTime(0.0) {}

void Metaheuristic::startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Metaheuristic::stopTimer() {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    executionTime = duration.count() / 1000000000.0;
} 