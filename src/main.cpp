#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "problem.hpp"
#include "constructive.hpp"
#include "local_search.hpp"
#include "iterated_local_search.hpp"
#include "simulated_annealing.hpp"
#include "tabu_search.hpp"
#include "genetic_algorithm.hpp"
#include "ant_colony.hpp"

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

int main(int argc, char* argv[]) {
    std::cout << "Starting program..." << std::endl;
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    try {
        std::cout << "Loading problem from file: " << argv[1] << std::endl;
        Problem problem(argv[1]);
        
        std::cout << "Problem size: " << problem.getNumJobs() << " jobs, "
                  << problem.getNumMachines() << " machines" << std::endl;
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
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Program completed successfully." << std::endl;
    return 0;
} 