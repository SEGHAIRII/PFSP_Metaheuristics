#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <string>
#include "problem.hpp"
#include "constructive.hpp"
#include "local_search.hpp"
#include "iterated_local_search.hpp"
#include "simulated_annealing.hpp"
#include "tabu_search.hpp"
#include "genetic_algorithm.hpp"
#include "ant_colony.hpp"
#include "grid_search.hpp"

void printResults(const std::string& method, const Solution& solution, double executionTime) {
    // Format execution time with appropriate units based on magnitude
    std::ostringstream timeOss;
    if (executionTime < 0.000001) { // Less than 1 microsecond
        timeOss << std::fixed << std::setprecision(2) << (executionTime * 1000000000.0) << " ns";
    } else if (executionTime < 0.001) { // Less than 1 millisecond
        timeOss << std::fixed << std::setprecision(2) << (executionTime * 1000000.0) << " µs";
    } else if (executionTime < 1.0) { // Less than 1 second
        timeOss << std::fixed << std::setprecision(2) << (executionTime * 1000.0) << " ms";
    } else { // 1 second or more
        timeOss << std::fixed << std::setprecision(3) << executionTime << " s";
    }
    std::string timeStr = timeOss.str();

    // Print header for this metaheuristic
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "Metaheuristic: " << method << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // Print makespan with high precision
    std::cout << "Makespan: " << std::fixed << std::setprecision(2) << solution.getMakespan() << std::endl;
    
    // Print execution time with maximum precision
    std::cout << "Execution Time: " << timeStr << std::endl;
    
    // Display the best found schedule in a more readable format
    std::cout << "Best Schedule:" << std::endl;
    const auto& perm = solution.getPermutation();
    std::cout << "  [";
    for (size_t i = 0; i < perm.size(); ++i) {
        std::cout << perm[i];
        if (i < perm.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <input_file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --grid-search       Run grid search on all algorithms" << std::endl;
    std::cout << "  --gs-ga             Run grid search on Genetic Algorithm only" << std::endl;
    std::cout << "  --gs-aco            Run grid search on Ant Colony Optimization only" << std::endl;
    std::cout << "  --gs-ils            Run grid search on Iterated Local Search only" << std::endl;
    std::cout << "  --gs-ls             Run grid search on Local Search only" << std::endl;
    std::cout << "  --gs-sa             Run grid search on Simulated Annealing only" << std::endl;
    std::cout << "  --gs-ts             Run grid search on Tabu Search only" << std::endl;
    std::cout << "  --help              Display this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameter customization (use commas to separate multiple values):" << std::endl;
    std::cout << "  --ga-pop=20,50,100       Set population sizes for GA" << std::endl;
    std::cout << "  --ga-gen=50,100,200      Set max generations for GA" << std::endl;
    std::cout << "  --ga-mut=0.05,0.1,0.2    Set mutation rates for GA" << std::endl;
    std::cout << "  --aco-ants=10,20,30      Set number of ants for ACO" << std::endl;
    std::cout << "  --aco-iter=50,100,150    Set max iterations for ACO" << std::endl;
    std::cout << "  --aco-evap=0.05,0.1,0.2  Set evaporation rates for ACO" << std::endl;
    std::cout << "  --aco-alpha=0.5,1.0,1.5  Set alpha values for ACO" << std::endl;
    std::cout << "  --aco-beta=1.0,2.0,3.0   Set beta values for ACO" << std::endl;
    std::cout << "  --ils-iter=50,100,200    Set max iterations for ILS" << std::endl;
    std::cout << "  --ils-perturb=2,3,5      Set perturbation strengths for ILS" << std::endl;
    std::cout << "  --ls-iter=500,1000,2000  Set max iterations for LS" << std::endl;
    std::cout << "  --sa-iter=500,1000,2000  Set max iterations for SA" << std::endl;
    std::cout << "  --sa-temp=500,1000,2000  Set initial temperatures for SA" << std::endl;
    std::cout << "  --sa-cool=0.9,0.95,0.98  Set cooling rates for SA" << std::endl;
    std::cout << "  --ts-iter=500,1000,2000  Set max iterations for TS" << std::endl;
    std::cout << "  --ts-tabu=5,10,20        Set tabu list sizes for TS" << std::endl;
}

// Helper function to parse comma-separated int values
std::vector<int> parseIntList(const std::string& str) {
    std::vector<int> result;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, ',')) {
        try {
            result.push_back(std::stoi(token));
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to parse integer: " << token << std::endl;
        }
    }
    
    return result;
}

// Helper function to parse comma-separated double values
std::vector<double> parseDoubleList(const std::string& str) {
    std::vector<double> result;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, ',')) {
        try {
            result.push_back(std::stod(token));
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to parse double: " << token << std::endl;
        }
    }
    
    return result;
}

int main(int argc, char* argv[]) {
    std::cout << "Starting program..." << std::endl;
    
    // Parse command line arguments
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string inputFile;
    bool runGridSearch = false;
    bool runGAGridSearch = false;
    bool runACOGridSearch = false;
    bool runILSGridSearch = false;
    bool runLSGridSearch = false;
    bool runSAGridSearch = false;
    bool runTSGridSearch = false;
    
    // Custom parameter values
    std::vector<int> gaPopSizes;
    std::vector<int> gaMaxGens;
    std::vector<double> gaMutRates;
    
    std::vector<int> acoAnts;
    std::vector<int> acoIters;
    std::vector<double> acoEvapRates;
    std::vector<double> acoAlphas;
    std::vector<double> acoBetas;
    
    std::vector<int> ilsIters;
    std::vector<int> ilsPerturbStrengths;
    
    std::vector<int> lsIters;
    
    std::vector<int> saIters;
    std::vector<double> saTemps;
    std::vector<double> saCoolRates;
    
    std::vector<int> tsIters;
    std::vector<int> tsTabuSizes;
    
    // Process command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--grid-search") {
            runGridSearch = true;
        } else if (arg == "--gs-ga") {
            runGAGridSearch = true;
        } else if (arg == "--gs-aco") {
            runACOGridSearch = true;
        } else if (arg == "--gs-ils") {
            runILSGridSearch = true;
        } else if (arg == "--gs-ls") {
            runLSGridSearch = true;
        } else if (arg == "--gs-sa") {
            runSAGridSearch = true;
        } else if (arg == "--gs-ts") {
            runTSGridSearch = true;
        } else if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg.find("--ga-pop=") == 0) {
            gaPopSizes = parseIntList(arg.substr(9));
        } else if (arg.find("--ga-gen=") == 0) {
            gaMaxGens = parseIntList(arg.substr(9));
        } else if (arg.find("--ga-mut=") == 0) {
            gaMutRates = parseDoubleList(arg.substr(9));
        } else if (arg.find("--aco-ants=") == 0) {
            acoAnts = parseIntList(arg.substr(11));
        } else if (arg.find("--aco-iter=") == 0) {
            acoIters = parseIntList(arg.substr(11));
        } else if (arg.find("--aco-evap=") == 0) {
            acoEvapRates = parseDoubleList(arg.substr(11));
        } else if (arg.find("--aco-alpha=") == 0) {
            acoAlphas = parseDoubleList(arg.substr(12));
        } else if (arg.find("--aco-beta=") == 0) {
            acoBetas = parseDoubleList(arg.substr(11));
        } else if (arg.find("--ils-iter=") == 0) {
            ilsIters = parseIntList(arg.substr(11));
        } else if (arg.find("--ils-perturb=") == 0) {
            ilsPerturbStrengths = parseIntList(arg.substr(14));
        } else if (arg.find("--ls-iter=") == 0) {
            lsIters = parseIntList(arg.substr(10));
        } else if (arg.find("--sa-iter=") == 0) {
            saIters = parseIntList(arg.substr(10));
        } else if (arg.find("--sa-temp=") == 0) {
            saTemps = parseDoubleList(arg.substr(10));
        } else if (arg.find("--sa-cool=") == 0) {
            saCoolRates = parseDoubleList(arg.substr(10));
        } else if (arg.find("--ts-iter=") == 0) {
            tsIters = parseIntList(arg.substr(10));
        } else if (arg.find("--ts-tabu=") == 0) {
            tsTabuSizes = parseIntList(arg.substr(10));
        } else {
            // Assume it's the input file
            inputFile = arg;
        }
    }
    
    if (inputFile.empty()) {
        std::cerr << "Error: Input file not specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        std::cout << "Loading problem from file: " << inputFile << std::endl;
        Problem problem(inputFile);
        
        std::cout << "Problem size: " << problem.getNumJobs() << " jobs, "
                  << problem.getNumMachines() << " machines" << std::endl;
        
        // If we're running grid search
        if (runGridSearch || runGAGridSearch || runACOGridSearch || runILSGridSearch || 
            runLSGridSearch || runSAGridSearch || runTSGridSearch) {
            
            GridSearch gridSearch(problem);
            
            if (runGridSearch) {
                // Run grid search on all algorithms with default or custom parameters
                
                // Create parameters with custom values if provided
                GeneticAlgorithmParams gaParams = gaPopSizes.empty() && gaMaxGens.empty() && gaMutRates.empty()
                    ? GeneticAlgorithmParams()
                    : GeneticAlgorithmParams(
                        gaPopSizes.empty() ? std::vector<int>{20, 50, 100} : gaPopSizes,
                        gaMaxGens.empty() ? std::vector<int>{50, 100, 200} : gaMaxGens,
                        gaMutRates.empty() ? std::vector<double>{0.05, 0.1, 0.2} : gaMutRates);
                
                AntColonyParams acoParams = acoAnts.empty() && acoIters.empty() && acoEvapRates.empty() && acoAlphas.empty() && acoBetas.empty()
                    ? AntColonyParams()
                    : AntColonyParams(
                        acoAnts.empty() ? std::vector<int>{10, 20, 30} : acoAnts,
                        acoIters.empty() ? std::vector<int>{50, 100, 150} : acoIters,
                        acoEvapRates.empty() ? std::vector<double>{0.05, 0.1, 0.2} : acoEvapRates,
                        acoAlphas.empty() ? std::vector<double>{0.5, 1.0, 1.5} : acoAlphas,
                        acoBetas.empty() ? std::vector<double>{1.0, 2.0, 3.0} : acoBetas);
                
                IteratedLocalSearchParams ilsParams = ilsIters.empty() && ilsPerturbStrengths.empty()
                    ? IteratedLocalSearchParams()
                    : IteratedLocalSearchParams(
                        ilsIters.empty() ? std::vector<int>{50, 100, 200} : ilsIters,
                        ilsPerturbStrengths.empty() ? std::vector<int>{2, 3, 5} : ilsPerturbStrengths);
                
                LocalSearchParams lsParams = lsIters.empty()
                    ? LocalSearchParams()
                    : LocalSearchParams(lsIters);
                
                SimulatedAnnealingParams saParams = saIters.empty() && saTemps.empty() && saCoolRates.empty()
                    ? SimulatedAnnealingParams()
                    : SimulatedAnnealingParams(
                        saIters.empty() ? std::vector<int>{500, 1000, 2000} : saIters,
                        saTemps.empty() ? std::vector<double>{500.0, 1000.0, 2000.0} : saTemps,
                        saCoolRates.empty() ? std::vector<double>{0.9, 0.95, 0.98} : saCoolRates);
                
                TabuSearchParams tsParams = tsIters.empty() && tsTabuSizes.empty()
                    ? TabuSearchParams()
                    : TabuSearchParams(
                        tsIters.empty() ? std::vector<int>{500, 1000, 2000} : tsIters,
                        tsTabuSizes.empty() ? std::vector<int>{5, 10, 20} : tsTabuSizes);
                
                // Run grid search with custom parameters
                gridSearch.runGeneticAlgorithm(gaParams);
                gridSearch.runAntColony(acoParams);
                gridSearch.runIteratedLocalSearch(ilsParams);
                gridSearch.runLocalSearch(lsParams);
                gridSearch.runSimulatedAnnealing(saParams);
                gridSearch.runTabuSearch(tsParams);
            } else {
                // Run grid search on selected algorithms only with custom parameters
                if (runGAGridSearch) {
                    GeneticAlgorithmParams gaParams = gaPopSizes.empty() && gaMaxGens.empty() && gaMutRates.empty()
                        ? GeneticAlgorithmParams()
                        : GeneticAlgorithmParams(
                            gaPopSizes.empty() ? std::vector<int>{20, 50, 100} : gaPopSizes,
                            gaMaxGens.empty() ? std::vector<int>{50, 100, 200} : gaMaxGens,
                            gaMutRates.empty() ? std::vector<double>{0.05, 0.1, 0.2} : gaMutRates);
                    gridSearch.runGeneticAlgorithm(gaParams);
                }
                
                if (runACOGridSearch) {
                    AntColonyParams acoParams = acoAnts.empty() && acoIters.empty() && acoEvapRates.empty() && acoAlphas.empty() && acoBetas.empty()
                        ? AntColonyParams()
                        : AntColonyParams(
                            acoAnts.empty() ? std::vector<int>{10, 20, 30} : acoAnts,
                            acoIters.empty() ? std::vector<int>{50, 100, 150} : acoIters,
                            acoEvapRates.empty() ? std::vector<double>{0.05, 0.1, 0.2} : acoEvapRates,
                            acoAlphas.empty() ? std::vector<double>{0.5, 1.0, 1.5} : acoAlphas,
                            acoBetas.empty() ? std::vector<double>{1.0, 2.0, 3.0} : acoBetas);
                    gridSearch.runAntColony(acoParams);
                }
                
                if (runILSGridSearch) {
                    IteratedLocalSearchParams ilsParams = ilsIters.empty() && ilsPerturbStrengths.empty()
                        ? IteratedLocalSearchParams()
                        : IteratedLocalSearchParams(
                            ilsIters.empty() ? std::vector<int>{50, 100, 200} : ilsIters,
                            ilsPerturbStrengths.empty() ? std::vector<int>{2, 3, 5} : ilsPerturbStrengths);
                    gridSearch.runIteratedLocalSearch(ilsParams);
                }
                
                if (runLSGridSearch) {
                    LocalSearchParams lsParams = lsIters.empty()
                        ? LocalSearchParams()
                        : LocalSearchParams(lsIters);
                    gridSearch.runLocalSearch(lsParams);
                }
                
                if (runSAGridSearch) {
                    SimulatedAnnealingParams saParams = saIters.empty() && saTemps.empty() && saCoolRates.empty()
                        ? SimulatedAnnealingParams()
                        : SimulatedAnnealingParams(
                            saIters.empty() ? std::vector<int>{500, 1000, 2000} : saIters,
                            saTemps.empty() ? std::vector<double>{500.0, 1000.0, 2000.0} : saTemps,
                            saCoolRates.empty() ? std::vector<double>{0.9, 0.95, 0.98} : saCoolRates);
                    gridSearch.runSimulatedAnnealing(saParams);
                }
                
                if (runTSGridSearch) {
                    TabuSearchParams tsParams = tsIters.empty() && tsTabuSizes.empty()
                        ? TabuSearchParams()
                        : TabuSearchParams(
                            tsIters.empty() ? std::vector<int>{500, 1000, 2000} : tsIters,
                            tsTabuSizes.empty() ? std::vector<int>{5, 10, 20} : tsTabuSizes);
                    gridSearch.runTabuSearch(tsParams);
                }
                
                // Print the best solution found
                std::cout << "\n" << std::string(80, '=') << std::endl;
                std::cout << "BEST SOLUTION FROM GRID SEARCH" << std::endl;
                std::cout << std::string(80, '=') << std::endl;
                std::cout << "Algorithm: " << gridSearch.getBestAlgorithm() << std::endl;
                std::cout << "Parameters: " << gridSearch.getBestParameters() << std::endl;
                std::cout << "Makespan: " << gridSearch.getBestSolution().getMakespan() << std::endl;
                
                // Print the best schedule
                std::cout << "Best Schedule: [";
                const auto& perm = gridSearch.getBestSolution().getPermutation();
                for (size_t i = 0; i < perm.size(); ++i) {
                    std::cout << perm[i];
                    if (i < perm.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]" << std::endl;
            }
        } else {
            // Run all metaheuristics with default parameters
            std::cout << std::string(80, '-') << std::endl;
            std::cout << std::setw(25) << "Method" << " | "
                      << std::setw(10) << "Makespan" << " | "
                      << std::setw(15) << "Time" << " | " << std::endl;
            std::cout << std::string(80, '-') << std::endl;
            
            // Run each metaheuristic
            std::cout << "Running Constructive..." << std::endl;
            Constructive constructive(problem);
            Solution constructiveSol = constructive.solve();
            printResults("Constructive", constructiveSol, constructive.getExecutionTime());
            
            std::cout << "Running Local Search..." << std::endl;
            LocalSearch localSearch(problem);
            Solution localSearchSol = localSearch.solve();
            printResults("Local Search", localSearchSol, localSearch.getExecutionTime());
            
            std::cout << "Running Iterated Local Search..." << std::endl;
            IteratedLocalSearch ils(problem);
            Solution ilsSol = ils.solve();
            printResults("Iterated Local Search", ilsSol, ils.getExecutionTime());
            
            std::cout << "Running Simulated Annealing..." << std::endl;
            SimulatedAnnealing sa(problem);
            Solution saSol = sa.solve();
            printResults("Simulated Annealing", saSol, sa.getExecutionTime());
            
            std::cout << "Running Tabu Search..." << std::endl;
            TabuSearch ts(problem);
            Solution tsSol = ts.solve();
            printResults("Tabu Search", tsSol, ts.getExecutionTime());
            
            std::cout << "Running Genetic Algorithm..." << std::endl;
            GeneticAlgorithm ga(problem);
            Solution gaSol = ga.solve();
            printResults("Genetic Algorithm", gaSol, ga.getExecutionTime());
            
            std::cout << "Running Ant Colony Optimization..." << std::endl;
            AntColony aco(problem);
            Solution acoSol = aco.solve();
            printResults("Ant Colony Optimization", acoSol, aco.getExecutionTime());
            
            std::cout << std::string(80, '-') << std::endl;
            
            // Print overall best solution
            std::vector<std::pair<std::string, Solution>> allSolutions = {
                {"Constructive", constructiveSol},
                {"Local Search", localSearchSol},
                {"Iterated Local Search", ilsSol},
                {"Simulated Annealing", saSol},
                {"Tabu Search", tsSol},
                {"Genetic Algorithm", gaSol},
                {"Ant Colony Optimization", acoSol}
            };
            
            auto bestSolution = std::min_element(
                allSolutions.begin(), 
                allSolutions.end(),
                [](const auto& a, const auto& b) {
                    return a.second.getMakespan() < b.second.getMakespan();
                }
            );
            
            std::cout << "Best overall solution: " << bestSolution->first 
                      << " with makespan " << bestSolution->second.getMakespan() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Program completed successfully." << std::endl;
    return 0;
} 