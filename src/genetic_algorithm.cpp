#include "genetic_algorithm.hpp"
#include <random>
#include <algorithm>

GeneticAlgorithm::GeneticAlgorithm(const Problem& problem) 
    : Metaheuristic(problem), gen(rd()) {}

Solution GeneticAlgorithm::solve() {
    startTimer();
    
    initializePopulation();
    int generation = 0;
    
    while (generation < MAX_GENERATIONS) {
        evaluatePopulation();
        selection();
        crossover();
        mutation();
        generation++;
    }
    
    // Find best solution in final population
    auto bestIndividual = std::min_element(population.begin(), population.end(),
        [](const Individual& a, const Individual& b) {
            return a.makespan < b.makespan;
        });
    
    bestSolution.setPermutation(bestIndividual->permutation);
    stopTimer();
    return bestSolution;
}

void GeneticAlgorithm::initializePopulation() {
    population.resize(POPULATION_SIZE);
    for (auto& individual : population) {
        individual.permutation.resize(problem.getNumJobs());
        std::iota(individual.permutation.begin(), individual.permutation.end(), 0);
        std::shuffle(individual.permutation.begin(), individual.permutation.end(), gen);
    }
}

void GeneticAlgorithm::evaluatePopulation() {
    for (auto& individual : population) {
        Solution temp(problem);
        temp.setPermutation(individual.permutation);
        individual.makespan = temp.getMakespan();
    }
}

void GeneticAlgorithm::selection() {
    // Tournament selection
    std::vector<Individual> newPopulation;
    std::uniform_int_distribution<> dis(0, POPULATION_SIZE - 1);
    
    while (newPopulation.size() < POPULATION_SIZE) {
        // Select two individuals for tournament
        int idx1 = dis(gen);
        int idx2 = dis(gen);
        
        // Keep the better one
        if (population[idx1].makespan < population[idx2].makespan) {
            newPopulation.push_back(population[idx1]);
        } else {
            newPopulation.push_back(population[idx2]);
        }
    }
    
    population = newPopulation;
}

void GeneticAlgorithm::crossover() {
    // Order Crossover (OX)
    for (size_t i = 0; i < population.size(); i += 2) {
        if (i + 1 >= population.size()) break;
        
        std::uniform_int_distribution<> dis(0, problem.getNumJobs() - 1);
        int start = dis(gen);
        int length = dis(gen);
        
        if (start + length > problem.getNumJobs()) {
            length = problem.getNumJobs() - start;
        }
        
        // Create offspring
        Individual child1, child2;
        child1.permutation.resize(problem.getNumJobs());
        child2.permutation.resize(problem.getNumJobs());
        
        // Copy segments
        for (int j = 0; j < length; ++j) {
            child1.permutation[start + j] = population[i].permutation[start + j];
            child2.permutation[start + j] = population[i + 1].permutation[start + j];
        }
        
        // Fill remaining positions
        std::vector<bool> used1(problem.getNumJobs(), false);
        std::vector<bool> used2(problem.getNumJobs(), false);
        
        for (int j = 0; j < length; ++j) {
            used1[population[i].permutation[start + j]] = true;
            used2[population[i + 1].permutation[start + j]] = true;
        }
        
        int pos1 = (start + length) % problem.getNumJobs();
        int pos2 = (start + length) % problem.getNumJobs();
        
        for (int j = 0; j < problem.getNumJobs(); ++j) {
            int curr1 = (start + length + j) % problem.getNumJobs();
            int curr2 = (start + length + j) % problem.getNumJobs();
            
            for (int k = 0; k < problem.getNumJobs(); ++k) {
                if (!used1[population[i + 1].permutation[k]]) {
                    child1.permutation[curr1] = population[i + 1].permutation[k];
                    used1[population[i + 1].permutation[k]] = true;
                    break;
                }
            }
            
            for (int k = 0; k < problem.getNumJobs(); ++k) {
                if (!used2[population[i].permutation[k]]) {
                    child2.permutation[curr2] = population[i].permutation[k];
                    used2[population[i].permutation[k]] = true;
                    break;
                }
            }
        }
        
        population[i] = child1;
        population[i + 1] = child2;
    }
}

void GeneticAlgorithm::mutation() {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> posDis(0, problem.getNumJobs() - 1);
    
    for (auto& individual : population) {
        if (dis(gen) < MUTATION_RATE) {
            // Swap mutation
            int pos1 = posDis(gen);
            int pos2 = posDis(gen);
            if (pos1 != pos2) {
                std::swap(individual.permutation[pos1], individual.permutation[pos2]);
            }
        }
    }
} 