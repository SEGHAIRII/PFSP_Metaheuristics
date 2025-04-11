#pragma once

#include "problem.hpp"
#include "solution.hpp"
#include "metaheuristic.hpp"
#include "constructive.hpp"
#include "local_search.hpp"
#include "iterated_local_search.hpp"
#include "simulated_annealing.hpp"
#include "tabu_search.hpp"
#include "genetic_algorithm.hpp"
#include "ant_colony.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <utility>
#include <sstream>

// Parameter ranges for different metaheuristics
struct GeneticAlgorithmParams {
    std::vector<int> populationSizes = {20, 50, 100};
    std::vector<int> maxGenerations = {50, 100, 200};
    std::vector<double> mutationRates = {0.05, 0.1, 0.2};

    // Default constructor uses default values above
    GeneticAlgorithmParams() = default;
    
    // Custom constructor for specifying parameter sets
    GeneticAlgorithmParams(const std::vector<int>& popSizes, 
                          const std::vector<int>& maxGens, 
                          const std::vector<double>& mutRates)
        : populationSizes(popSizes), maxGenerations(maxGens), mutationRates(mutRates) {}
};

struct AntColonyParams {
    std::vector<int> numAnts = {10, 20, 30};
    std::vector<int> maxIterations = {50, 100, 150};
    std::vector<double> evaporationRates = {0.05, 0.1, 0.2};
    std::vector<double> alphaValues = {0.5, 1.0, 1.5};
    std::vector<double> betaValues = {1.0, 2.0, 3.0};

    // Default constructor uses default values above
    AntColonyParams() = default;
    
    // Custom constructor for specifying parameter sets
    AntColonyParams(const std::vector<int>& ants,
                   const std::vector<int>& iterations,
                   const std::vector<double>& evapRates,
                   const std::vector<double>& alphas,
                   const std::vector<double>& betas)
        : numAnts(ants), maxIterations(iterations), evaporationRates(evapRates),
          alphaValues(alphas), betaValues(betas) {}
};

struct IteratedLocalSearchParams {
    std::vector<int> maxIterations = {50, 100, 200};
    std::vector<int> perturbationStrengths = {2, 3, 5};

    // Default constructor uses default values above
    IteratedLocalSearchParams() = default;
    
    // Custom constructor for specifying parameter sets
    IteratedLocalSearchParams(const std::vector<int>& iterations,
                             const std::vector<int>& perturbStrengths)
        : maxIterations(iterations), perturbationStrengths(perturbStrengths) {}
};

struct LocalSearchParams {
    std::vector<int> maxIterations = {500, 1000, 2000};

    // Default constructor uses default values above
    LocalSearchParams() = default;
    
    // Custom constructor for specifying parameter sets
    LocalSearchParams(const std::vector<int>& iterations)
        : maxIterations(iterations) {}
};

struct SimulatedAnnealingParams {
    std::vector<int> maxIterations = {500, 1000, 2000};
    std::vector<double> initialTemperatures = {500.0, 1000.0, 2000.0};
    std::vector<double> coolingRates = {0.9, 0.95, 0.98};

    // Default constructor uses default values above
    SimulatedAnnealingParams() = default;
    
    // Custom constructor for specifying parameter sets
    SimulatedAnnealingParams(const std::vector<int>& iterations,
                            const std::vector<double>& initTemps,
                            const std::vector<double>& cooling)
        : maxIterations(iterations), initialTemperatures(initTemps), coolingRates(cooling) {}
};

struct TabuSearchParams {
    std::vector<int> maxIterations = {500, 1000, 2000};
    std::vector<int> tabuListSizes = {5, 10, 20};

    // Default constructor uses default values above
    TabuSearchParams() = default;
    
    // Custom constructor for specifying parameter sets
    TabuSearchParams(const std::vector<int>& iterations,
                    const std::vector<int>& tabuSizes)
        : maxIterations(iterations), tabuListSizes(tabuSizes) {}
};

class GridSearch {
public:
    GridSearch(const Problem& problem);
    
    // Run grid search for a specific algorithm
    void runGeneticAlgorithm(const GeneticAlgorithmParams& params);
    void runAntColony(const AntColonyParams& params);
    void runIteratedLocalSearch(const IteratedLocalSearchParams& params);
    void runLocalSearch(const LocalSearchParams& params);
    void runSimulatedAnnealing(const SimulatedAnnealingParams& params);
    void runTabuSearch(const TabuSearchParams& params);
    
    // Run grid search for all algorithms with default parameter ranges
    void runAll();
    
    // Get the overall best solution
    Solution getBestSolution() const;
    std::string getBestAlgorithm() const;
    std::string getBestParameters() const;
    
private:
    const Problem& problem;
    Solution bestSolution;
    std::string bestAlgorithm;
    std::string bestParameters;
    
    // Helper method to update the best solution
    void updateBestSolution(const std::string& algorithm, const std::string& params, const Solution& solution);
    
    // Format parameter string
    template<typename T>
    std::string formatParam(const std::string& name, const T& value) {
        std::ostringstream oss;
        oss << name << "=" << value;
        return oss.str();
    }
}; 