#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <random>

class GeneticAlgorithm : public Metaheuristic {
public:
    // Default constructor
    GeneticAlgorithm(const Problem& problem);
    
    // Parameterized constructor
    GeneticAlgorithm(const Problem& problem, int populationSize, int maxGenerations, double mutationRate);
    
    Solution solve() override;
    
private:
    // Components
    struct Individual {
        std::vector<int> permutation;
        int makespan;
    };
    
    void initializePopulation();
    void evaluatePopulation();
    void selection();
    void crossover();
    void mutation();
    
    // Parameters
    int populationSize;
    int maxGenerations;
    double mutationRate;
    
    // Population
    std::vector<Individual> population;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
}; 