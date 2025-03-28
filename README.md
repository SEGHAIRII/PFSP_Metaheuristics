# Permutation Flowshop Problem Metaheuristics

This project implements various metaheuristic algorithms to solve the Permutation Flowshop Problem (PFSP). The PFSP is a scheduling problem where n jobs must be processed on m machines in the same order, with the objective of minimizing the makespan.

## Implemented Metaheuristics

1. Constructive Heuristic
   - NEH Algorithm
   - Random Permutation
   - Shortest Processing Time

2. Local Search
   - Swap Neighborhood
   - Insert Neighborhood
   - Reverse Neighborhood

3. Iterated Local Search
   - Perturbation
   - Local Search

4. Simulated Annealing
   - Temperature-based acceptance
   - Cooling schedule

5. Tabu Search
   - Tabu list management
   - Neighborhood exploration

6. Genetic Algorithm
   - Population management
   - Crossover and mutation operators
  
7. Ant Colony Optimization
   - Pheromone-based construction
   - Heuristic information
   - Pheromone evaporation and update


## Building the Project

### Prerequisites

- CMake
- C++17 compatible compiler

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

The program takes an input file as a command-line argument. The input file should contain the processing times matrix, where:
- Each line represents a machine
- Each column represents a job
- Each cell contains the processing time of the job on that machine

Example input file format:
```
3 3
2 3 4
3 4 2
4 2 3
```

Run the program:
```bash
./pfsp input.txt
```

## Output

The program will output:
- Problem size information
- Results for each metaheuristic:
  - Method name
  - Best makespan found
  - Execution time

