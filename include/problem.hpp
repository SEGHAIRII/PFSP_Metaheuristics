#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

class Problem {
public:
    Problem(const std::string& filename);
    
    // Getters
    int getNumMachines() const { return numMachines; }
    int getNumJobs() const { return numJobs; }
    const std::vector<std::vector<int>>& getProcessingTimes() const { return processingTimes; }
    
    // Calculate makespan for a given permutation
    int calculateMakespan(const std::vector<int>& permutation) const;
    
private:
    int numMachines;
    int numJobs;
    std::vector<std::vector<int>> processingTimes; // [machine][job]
    
    void loadFromFile(const std::string& filename);
}; 