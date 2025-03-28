#include "problem.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

Problem::Problem(const std::string& filename) {
    loadFromFile(filename);
}

void Problem::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    std::vector<std::vector<int>> tempMatrix;
    
    // Read first line to get number of jobs and machines
    std::getline(file, line);
    std::istringstream iss(line);
    iss >> numJobs >> numMachines;
    
    // Initialize processing times matrix
    processingTimes.resize(numMachines, std::vector<int>(numJobs));
    
    // Read processing times
    for (int i = 0; i < numMachines; ++i) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Not enough lines in input file");
        }
        std::istringstream iss(line);
        for (int j = 0; j < numJobs; ++j) {
            if (!(iss >> processingTimes[i][j])) {
                throw std::runtime_error("Not enough values in line " + std::to_string(i + 2));
            }
        }
        int extraValue;
        if (iss >> extraValue) {
            throw std::runtime_error("Too many values in line " + std::to_string(i + 2));
        }
    }
    
    std::string extraLine;
    if (std::getline(file, extraLine)) {
        throw std::runtime_error("Too many lines in input file");
    }
}

int Problem::calculateMakespan(const std::vector<int>& permutation) const {
    if (permutation.size() != numJobs) {
        throw std::runtime_error("Invalid permutation size");
    }

    // Initialize completion times matrix
    std::vector<std::vector<int>> completionTimes(numMachines, std::vector<int>(numJobs, 0));
    
    // Calculate completion times
    for (int i = 0; i < numMachines; ++i) {
        for (int j = 0; j < numJobs; ++j) {
            int job = permutation[j];
            int prevMachineTime = (i > 0) ? completionTimes[i-1][j] : 0;
            int prevJobTime = (j > 0) ? completionTimes[i][j-1] : 0;
            completionTimes[i][j] = std::max(prevMachineTime, prevJobTime) + processingTimes[i][job];
        }
    }
    
    return completionTimes[numMachines-1][numJobs-1];
} 