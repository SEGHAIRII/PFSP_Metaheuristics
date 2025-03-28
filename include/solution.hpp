#pragma once

#include <vector>
#include <memory>
#include "problem.hpp"

class Solution {
public:
    Solution(const Problem& problem);
    
    // Copy constructor and assignment operator
    Solution(const Solution& other);
    Solution& operator=(const Solution& other);
    
    // Getters and setters
    const std::vector<int>& getPermutation() const { return permutation; }
    int getMakespan() const { return makespan; }
    void setPermutation(const std::vector<int>& newPermutation);
    
    // Solution manipulation
    void swap(int i, int j);
    void insert(int i, int j);
    void reverse(int i, int j);
    
    // Solution evaluation
    void evaluate();
    
private:
    const Problem& problem;
    std::vector<int> permutation;
    int makespan;
}; 