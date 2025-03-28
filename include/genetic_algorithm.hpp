#pragma once

#include "metaheuristic.hpp"
#include <vector>
#include <random>

class GeneticAlgorithm : public Metaheuristic {
public:
    GeneticAlgorithm(const Problem& problem);
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
    static constexpr int POPULATION_SIZE = 50;
    static constexpr int MAX_GENERATIONS = 100;
    static constexpr double MUTATION_RATE = 0.1;
    
    // Population
    std::vector<Individual> population;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
}; 