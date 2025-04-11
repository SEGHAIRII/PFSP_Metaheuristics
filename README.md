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
2 3 4
3 4 2
4 2 3
```

Run the program:
```bash
./pfsp input.txt
```

### Grid Search

The program also supports grid search for parameter tuning. You can run grid search on specific metaheuristics or all of them:

```bash
# Run grid search on all metaheuristics
./pfsp --grid-search input.txt

# Run grid search on specific metaheuristics
./pfsp --gs-ga input.txt  # Genetic Algorithm
./pfsp --gs-aco input.txt # Ant Colony Optimization
./pfsp --gs-ils input.txt # Iterated Local Search
./pfsp --gs-ls input.txt  # Local Search
./pfsp --gs-sa input.txt  # Simulated Annealing
./pfsp --gs-ts input.txt  # Tabu Search
```

#### Custom Parameter Ranges

You can also specify custom parameter ranges to test in the grid search:

```bash
# Customize GA parameters
./pfsp --gs-ga --ga-pop=20,50,100 --ga-gen=50,100,200 --ga-mut=0.05,0.1,0.2 input.txt

# Customize ACO parameters
./pfsp --gs-aco --aco-ants=10,20,30 --aco-alpha=0.5,1.0,1.5 input.txt

# Customize SA parameters
./pfsp --gs-sa --sa-temp=500,1000,2000 --sa-cool=0.9,0.95,0.98 input.txt
```

Available parameter customization options:
- `--ga-pop=20,50,100` - Set population sizes for Genetic Algorithm
- `--ga-gen=50,100,200` - Set max generations for Genetic Algorithm
- `--ga-mut=0.05,0.1,0.2` - Set mutation rates for Genetic Algorithm
- `--aco-ants=10,20,30` - Set number of ants for Ant Colony Optimization
- `--aco-iter=50,100,150` - Set max iterations for Ant Colony Optimization
- `--aco-evap=0.05,0.1,0.2` - Set evaporation rates for Ant Colony Optimization
- `--aco-alpha=0.5,1.0,1.5` - Set alpha values for Ant Colony Optimization
- `--aco-beta=1.0,2.0,3.0` - Set beta values for Ant Colony Optimization
- `--ils-iter=50,100,200` - Set max iterations for Iterated Local Search
- `--ils-perturb=2,3,5` - Set perturbation strengths for Iterated Local Search
- `--ls-iter=500,1000,2000` - Set max iterations for Local Search
- `--sa-iter=500,1000,2000` - Set max iterations for Simulated Annealing
- `--sa-temp=500,1000,2000` - Set initial temperatures for Simulated Annealing
- `--sa-cool=0.9,0.95,0.98` - Set cooling rates for Simulated Annealing
- `--ts-iter=500,1000,2000` - Set max iterations for Tabu Search
- `--ts-tabu=5,10,20` - Set tabu list sizes for Tabu Search

For batch processing of multiple input files with grid search, use the provided script:

```bash
# Run grid search on all data files for all metaheuristics
./run_all.sh --grid-search

# Run grid search on all data files for specific metaheuristics with custom parameters
./run_all.sh --gs-ga --ga-pop=10,25,50 --ga-mut=0.05,0.15,0.25
```

## Output

The program will output:
- Problem size information
- Results for each metaheuristic:
  - Method name
  - Best makespan found
  - Execution time

