#include "grid_search.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

GridSearch::GridSearch(const Problem& problem) 
    : problem(problem), bestSolution(problem) {}

void GridSearch::updateBestSolution(const std::string& algorithm, const std::string& params, const Solution& solution) {
    if (bestSolution.getMakespan() == 0 || solution.getMakespan() < bestSolution.getMakespan()) {
        bestSolution = solution;
        bestAlgorithm = algorithm;
        bestParameters = params;
    }
}

Solution GridSearch::getBestSolution() const {
    return bestSolution;
}

std::string GridSearch::getBestAlgorithm() const {
    return bestAlgorithm;
}

std::string GridSearch::getBestParameters() const {
    return bestParameters;
}

void GridSearch::runGeneticAlgorithm(const GeneticAlgorithmParams& params) {
    std::cout << "\nRunning Genetic Algorithm Grid Search..." << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Population" << " | "
              << std::setw(15) << "Generations" << " | "
              << std::setw(15) << "Mutation Rate" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int populationSize : params.populationSizes) {
        for (int maxGenerations : params.maxGenerations) {
            for (double mutationRate : params.mutationRates) {
                // Create GA with the specific parameters
                GeneticAlgorithm ga(problem, populationSize, maxGenerations, mutationRate);
                Solution solution = ga.solve();
                double executionTime = ga.getExecutionTime();
                
                std::ostringstream paramStream;
                paramStream << "pop=" << populationSize << ",gen=" << maxGenerations << ",mut=" << mutationRate;
                std::string paramString = paramStream.str();
                
                std::cout << std::left << std::setw(15) << populationSize << " | "
                          << std::setw(15) << maxGenerations << " | "
                          << std::setw(15) << mutationRate << " | "
                          << std::setw(15) << solution.getMakespan() << " | "
                          << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
                
                updateBestSolution("Genetic Algorithm", paramString, solution);
            }
        }
    }
}

void GridSearch::runAntColony(const AntColonyParams& params) {
    std::cout << "\nRunning Ant Colony Optimization Grid Search..." << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::left << std::setw(10) << "Ants" << " | "
              << std::setw(10) << "Iterations" << " | "
              << std::setw(10) << "Evap Rate" << " | "
              << std::setw(10) << "Alpha" << " | "
              << std::setw(10) << "Beta" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    for (int numAnts : params.numAnts) {
        for (int maxIterations : params.maxIterations) {
            for (double evapRate : params.evaporationRates) {
                for (double alpha : params.alphaValues) {
                    for (double beta : params.betaValues) {
                        // Create ACO with specific parameters
                        AntColony aco(problem, numAnts, maxIterations, evapRate, alpha, beta);
                        Solution solution = aco.solve();
                        double executionTime = aco.getExecutionTime();
                        
                        std::ostringstream paramStream;
                        paramStream << "ants=" << numAnts << ",iter=" << maxIterations 
                                  << ",evap=" << evapRate << ",alpha=" << alpha << ",beta=" << beta;
                        std::string paramString = paramStream.str();
                        
                        std::cout << std::left << std::setw(10) << numAnts << " | "
                                  << std::setw(10) << maxIterations << " | "
                                  << std::setw(10) << evapRate << " | "
                                  << std::setw(10) << alpha << " | "
                                  << std::setw(10) << beta << " | "
                                  << std::setw(15) << solution.getMakespan() << " | "
                                  << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
                        
                        updateBestSolution("Ant Colony", paramString, solution);
                    }
                }
            }
        }
    }
}

void GridSearch::runIteratedLocalSearch(const IteratedLocalSearchParams& params) {
    std::cout << "\nRunning Iterated Local Search Grid Search..." << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Iterations" << " | "
              << std::setw(15) << "Perturb Str" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int maxIterations : params.maxIterations) {
        for (int perturbationStrength : params.perturbationStrengths) {
            // Create ILS with specific parameters
            IteratedLocalSearch ils(problem, maxIterations, perturbationStrength);
            Solution solution = ils.solve();
            double executionTime = ils.getExecutionTime();
            
            std::ostringstream paramStream;
            paramStream << "iter=" << maxIterations << ",perturb=" << perturbationStrength;
            std::string paramString = paramStream.str();
            
            std::cout << std::left << std::setw(15) << maxIterations << " | "
                      << std::setw(15) << perturbationStrength << " | "
                      << std::setw(15) << solution.getMakespan() << " | "
                      << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
            
            updateBestSolution("Iterated Local Search", paramString, solution);
        }
    }
}

void GridSearch::runLocalSearch(const LocalSearchParams& params) {
    std::cout << "\nRunning Local Search Grid Search..." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Iterations" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (int maxIterations : params.maxIterations) {
        // Create LS with specific parameters
        LocalSearch ls(problem, maxIterations);
        Solution solution = ls.solve();
        double executionTime = ls.getExecutionTime();
        
        std::ostringstream paramStream;
        paramStream << "iter=" << maxIterations;
        std::string paramString = paramStream.str();
        
        std::cout << std::left << std::setw(15) << maxIterations << " | "
                  << std::setw(15) << solution.getMakespan() << " | "
                  << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
        
        updateBestSolution("Local Search", paramString, solution);
    }
}

void GridSearch::runSimulatedAnnealing(const SimulatedAnnealingParams& params) {
    std::cout << "\nRunning Simulated Annealing Grid Search..." << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Iterations" << " | "
              << std::setw(15) << "Init Temp" << " | "
              << std::setw(15) << "Cooling Rate" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int maxIterations : params.maxIterations) {
        for (double initialTemp : params.initialTemperatures) {
            for (double coolingRate : params.coolingRates) {
                // Create SA with specific parameters
                SimulatedAnnealing sa(problem, maxIterations, initialTemp, coolingRate);
                Solution solution = sa.solve();
                double executionTime = sa.getExecutionTime();
                
                std::ostringstream paramStream;
                paramStream << "iter=" << maxIterations << ",temp=" << initialTemp << ",cool=" << coolingRate;
                std::string paramString = paramStream.str();
                
                std::cout << std::left << std::setw(15) << maxIterations << " | "
                          << std::setw(15) << initialTemp << " | "
                          << std::setw(15) << coolingRate << " | "
                          << std::setw(15) << solution.getMakespan() << " | "
                          << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
                
                updateBestSolution("Simulated Annealing", paramString, solution);
            }
        }
    }
}

void GridSearch::runTabuSearch(const TabuSearchParams& params) {
    std::cout << "\nRunning Tabu Search Grid Search..." << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Iterations" << " | "
              << std::setw(15) << "Tabu List Size" << " | "
              << std::setw(15) << "Makespan" << " | "
              << std::setw(15) << "Time (s)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int maxIterations : params.maxIterations) {
        for (int tabuListSize : params.tabuListSizes) {
            // Create TS with specific parameters
            TabuSearch ts(problem, maxIterations, tabuListSize);
            Solution solution = ts.solve();
            double executionTime = ts.getExecutionTime();
            
            std::ostringstream paramStream;
            paramStream << "iter=" << maxIterations << ",tabu=" << tabuListSize;
            std::string paramString = paramStream.str();
            
            std::cout << std::left << std::setw(15) << maxIterations << " | "
                      << std::setw(15) << tabuListSize << " | "
                      << std::setw(15) << solution.getMakespan() << " | "
                      << std::setw(15) << std::fixed << std::setprecision(6) << executionTime << std::endl;
            
            updateBestSolution("Tabu Search", paramString, solution);
        }
    }
}

void GridSearch::runAll() {
    // Run grid search for all algorithms with default parameters
    GeneticAlgorithmParams gaParams;
    AntColonyParams acoParams;
    IteratedLocalSearchParams ilsParams;
    LocalSearchParams lsParams;
    SimulatedAnnealingParams saParams;
    TabuSearchParams tsParams;
    
    runGeneticAlgorithm(gaParams);
    runAntColony(acoParams);
    runIteratedLocalSearch(ilsParams);
    runLocalSearch(lsParams);
    runSimulatedAnnealing(saParams);
    runTabuSearch(tsParams);
    
    // Print overall best solution
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "OVERALL BEST SOLUTION" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Algorithm: " << bestAlgorithm << std::endl;
    std::cout << "Parameters: " << bestParameters << std::endl;
    std::cout << "Makespan: " << bestSolution.getMakespan() << std::endl;
    
    // Print the best schedule
    std::cout << "Best Schedule: [";
    const auto& perm = bestSolution.getPermutation();
    for (size_t i = 0; i < perm.size(); ++i) {
        std::cout << perm[i];
        if (i < perm.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
} 