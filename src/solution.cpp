#include "solution.hpp"
#include <algorithm>
#include <random>

Solution::Solution(const Problem& problem) : problem(problem) {
    // Initialize with identity permutation
    permutation.resize(problem.getNumJobs());
    std::iota(permutation.begin(), permutation.end(), 0);
    evaluate();
}

// Copy constructor
Solution::Solution(const Solution& other) 
    : problem(other.problem), 
      permutation(other.permutation), 
      makespan(other.makespan) {}

// Assignment operator
Solution& Solution::operator=(const Solution& other) {
    if (this != &other) {
        // Note: We can't change the problem reference after construction
        permutation = other.permutation;
        makespan = other.makespan;
    }
    return *this;
}

void Solution::setPermutation(const std::vector<int>& newPermutation) {
    if (newPermutation.size() != problem.getNumJobs()) {
        throw std::runtime_error("Invalid permutation size: got " + 
                               std::to_string(newPermutation.size()) + 
                               ", expected " + 
                               std::to_string(problem.getNumJobs()));
    }
    
    // Check if all jobs are in the permutation
    std::vector<bool> jobsPresent(problem.getNumJobs(), false);
    for (int job : newPermutation) {
        if (job < 0 || job >= problem.getNumJobs()) {
            throw std::runtime_error("Invalid job index in permutation: " + std::to_string(job));
        }
        if (jobsPresent[job]) {
            throw std::runtime_error("Duplicate job in permutation: " + std::to_string(job));
        }
        jobsPresent[job] = true;
    }
    
    for (int j = 0; j < problem.getNumJobs(); ++j) {
        if (!jobsPresent[j]) {
            throw std::runtime_error("Missing job in permutation: " + std::to_string(j));
        }
    }
    
    permutation = newPermutation;
    evaluate();
}

void Solution::swap(int i, int j) {
    std::swap(permutation[i], permutation[j]);
    evaluate();
}

void Solution::insert(int i, int j) {
    if (i < j) {
        int temp = permutation[i];
        for (int k = i; k < j; ++k) {
            permutation[k] = permutation[k + 1];
        }
        permutation[j] = temp;
    } else if (i > j) {
        int temp = permutation[i];
        for (int k = i; k > j; --k) {
            permutation[k] = permutation[k - 1];
        }
        permutation[j] = temp;
    }
    evaluate();
}

void Solution::reverse(int i, int j) {
    if (i > j) std::swap(i, j);
    std::reverse(permutation.begin() + i, permutation.begin() + j + 1);
    evaluate();
}

void Solution::evaluate() {
    makespan = problem.calculateMakespan(permutation);
} 