#include "constructive.hpp"
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>

Constructive::Constructive(const Problem& problem) : Metaheuristic(problem) {}

Solution Constructive::solve() {
    startTimer();
    
    // Try different constructive methods and keep the best solution
    std::vector<std::vector<int>> permutations = {
        nehAlgorithm(),
        randomPermutation(),
        shortestProcessingTime()
    };
    
    Solution best(problem);
    int bestMakespan = std::numeric_limits<int>::max();
    
    for (const auto& perm : permutations) {
        try {
            Solution current(problem);
            current.setPermutation(perm);
            if (current.getMakespan() < bestMakespan) {
                bestMakespan = current.getMakespan();
                best = current;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating permutation: " << e.what() << std::endl;
        }
    }
    
    bestSolution = best;
    stopTimer();
    return bestSolution;
}

std::vector<int> Constructive::nehAlgorithm() {
    int n = problem.getNumJobs();
    int m = problem.getNumMachines();
    
    // If we have less than 2 jobs, return identity permutation
    if (n < 2) {
        std::vector<int> permutation(n);
        std::iota(permutation.begin(), permutation.end(), 0);
        return permutation;
    }
    
    // Calculate total processing time for each job
    std::vector<std::pair<int, int>> jobTimes(n);
    const auto& times = problem.getProcessingTimes();
    
    for (int j = 0; j < n; ++j) {
        int totalTime = 0;
        for (int i = 0; i < m; ++i) {
            totalTime += times[i][j];
        }
        jobTimes[j] = {totalTime, j};
    }
    
    // Sort jobs by decreasing total processing time
    std::sort(jobTimes.begin(), jobTimes.end(), std::greater<>());
    
    // Start with the first job
    std::vector<int> partial;
    partial.push_back(jobTimes[0].second);
    
    // Insert remaining jobs one by one
    for (int i = 1; i < n; ++i) {
        int job = jobTimes[i].second;
        int bestPos = 0;
        int bestMakespan = std::numeric_limits<int>::max();
        
        // Try all possible positions for the current job
        for (size_t pos = 0; pos <= partial.size(); ++pos) {
            // Create a complete permutation with all jobs
            std::vector<int> complete(n);
            std::vector<bool> used(n, false);
            
            // Add jobs from the partial permutation
            int idx = 0;
            for (size_t j = 0; j < partial.size(); ++j) {
                if (j == pos) {
                    complete[idx++] = job;
                }
                complete[idx++] = partial[j];
                used[partial[j]] = true;
            }
            
            // If job should be at the end
            if (pos == partial.size()) {
                complete[idx++] = job;
            }
            
            used[job] = true;
            
            // Add remaining unused jobs
            for (int j = 0; j < n; ++j) {
                if (!used[j]) {
                    complete[idx++] = j;
                }
            }
            
            try {
                Solution current(problem);
                current.setPermutation(complete);
                int makespan = current.getMakespan();
                
                if (makespan < bestMakespan) {
                    bestMakespan = makespan;
                    bestPos = pos;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in NEH: " << e.what() << std::endl;
                std::vector<int> fallback(n);
                std::iota(fallback.begin(), fallback.end(), 0);
                return fallback;
            }
        }
        
        // Insert the job at the best position in the partial permutation
        partial.insert(partial.begin() + bestPos, job);
    }
    
    // Generate the final complete permutation
    std::vector<int> finalPerm;
    std::vector<bool> used(n, false);
    
    // First add all jobs from the partial solution
    for (int job : partial) {
        finalPerm.push_back(job);
        used[job] = true;
    }
    
    // Add any remaining jobs (should be none if all went well)
    for (int j = 0; j < n; ++j) {
        if (!used[j]) {
            finalPerm.push_back(j);
        }
    }
    
    return finalPerm;
}

std::vector<int> Constructive::randomPermutation() {
    std::vector<int> permutation(problem.getNumJobs());
    std::iota(permutation.begin(), permutation.end(), 0);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(permutation.begin(), permutation.end(), gen);
    
    return permutation;
}

std::vector<int> Constructive::shortestProcessingTime() {
    int n = problem.getNumJobs();
    std::vector<int> permutation;
    permutation.reserve(n);
    
    // Calculate average processing time for each job
    std::vector<std::pair<double, int>> jobTimes(n);
    for (int j = 0; j < n; ++j) {
        double avgTime = 0.0;
        for (int m = 0; m < problem.getNumMachines(); ++m) {
            avgTime += problem.getProcessingTimes()[m][j];
        }
        avgTime /= problem.getNumMachines();
        jobTimes[j] = {avgTime, j};
    }
    
    // Sort jobs by increasing average processing time
    std::sort(jobTimes.begin(), jobTimes.end());
    
    // Create permutation
    for (const auto& job : jobTimes) {
        permutation.push_back(job.second);
    }
    
    return permutation;
} 