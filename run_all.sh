#!/bin/bash

# Colors for better readability
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Create logs directory if it doesn't exist
mkdir -p logs

# Find the executable
EXECUTABLE="./build/pfsp"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${YELLOW}Executable not found at $EXECUTABLE${NC}"
    echo -e "${YELLOW}Building the project...${NC}"
    mkdir -p build
    cd build
    cmake ..
    make
    cd ..
    
    if [ ! -f "$EXECUTABLE" ]; then
        echo -e "${YELLOW}Failed to build. Please build the project manually.${NC}"
        exit 1
    fi
fi

# Process command line arguments
RUN_GRID_SEARCH=false
GRID_SEARCH_ARGS=""
CUSTOM_PARAMS=""

print_help() {
    echo -e "${GREEN}PFSP Metaheuristics Runner${NC}"
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  --grid-search       Run grid search on all algorithms"
    echo "  --gs-ga             Run grid search on Genetic Algorithm only"
    echo "  --gs-aco            Run grid search on Ant Colony Optimization only"
    echo "  --gs-ils            Run grid search on Iterated Local Search only"
    echo "  --gs-ls             Run grid search on Local Search only"
    echo "  --gs-sa             Run grid search on Simulated Annealing only"
    echo "  --gs-ts             Run grid search on Tabu Search only"
    echo
    echo "Custom parameter ranges (use commas to separate values):"
    echo "  --ga-pop=20,50,100       Set population sizes for GA"
    echo "  --ga-gen=50,100,200      Set max generations for GA"
    echo "  --ga-mut=0.05,0.1,0.2    Set mutation rates for GA"
    echo "  --aco-ants=10,20,30      Set number of ants for ACO"
    echo "  --aco-iter=50,100,150    Set max iterations for ACO"
    echo "  --aco-evap=0.05,0.1,0.2  Set evaporation rates for ACO"
    echo "  --aco-alpha=0.5,1.0,1.5  Set alpha values for ACO"
    echo "  --aco-beta=1.0,2.0,3.0   Set beta values for ACO"
    echo "  --ils-iter=50,100,200    Set max iterations for ILS"
    echo "  --ils-perturb=2,3,5      Set perturbation strengths for ILS"
    echo "  --ls-iter=500,1000,2000  Set max iterations for LS"
    echo "  --sa-iter=500,1000,2000  Set max iterations for SA"
    echo "  --sa-temp=500,1000,2000  Set initial temperatures for SA"
    echo "  --sa-cool=0.9,0.95,0.98  Set cooling rates for SA"
    echo "  --ts-iter=500,1000,2000  Set max iterations for TS"
    echo "  --ts-tabu=5,10,20        Set tabu list sizes for TS"
    echo "  --help                   Display this help message"
}

while [[ $# -gt 0 ]]; do
    case $1 in
        --grid-search)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="--grid-search"
            shift
            ;;
        --gs-ga)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-ga"
            shift
            ;;
        --gs-aco)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-aco"
            shift
            ;;
        --gs-ils)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-ils"
            shift
            ;;
        --gs-ls)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-ls"
            shift
            ;;
        --gs-sa)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-sa"
            shift
            ;;
        --gs-ts)
            RUN_GRID_SEARCH=true
            GRID_SEARCH_ARGS="$GRID_SEARCH_ARGS --gs-ts"
            shift
            ;;
        --ga-pop=*|--ga-gen=*|--ga-mut=*|--aco-ants=*|--aco-iter=*|--aco-evap=*|--aco-alpha=*|--aco-beta=*|--ils-iter=*|--ils-perturb=*|--ls-iter=*|--sa-iter=*|--sa-temp=*|--sa-cool=*|--ts-iter=*|--ts-tabu=*)
            CUSTOM_PARAMS="$CUSTOM_PARAMS $1"
            shift
            ;;
        --help)
            print_help
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Count data files
DATA_FILES=( $(find data -name "*.txt" | sort) )
TOTAL=${#DATA_FILES[@]}
COUNTER=1

# Process each data file
for data_file in "${DATA_FILES[@]}"; do
    file_name=$(basename "$data_file")
    
    if [ "$RUN_GRID_SEARCH" = true ]; then
        log_file="logs/${file_name%.txt}_grid_search_results.log"
        echo -e "${GREEN}[$COUNTER/$TOTAL] Running grid search on $data_file${NC}"
    else
        log_file="logs/${file_name%.txt}_results.log"
        echo -e "${GREEN}[$COUNTER/$TOTAL] Processing $data_file${NC}"
    fi
    
    echo -e "${BLUE}Results will be saved to $log_file${NC}"
    
    # Run the program and save output to log file
    echo "=======================================================" > "$log_file"
    if [ "$RUN_GRID_SEARCH" = true ]; then
        echo "Running grid search metaheuristics on $file_name" >> "$log_file"
    else
        echo "Running metaheuristics on $file_name" >> "$log_file"
    fi
    echo "=======================================================" >> "$log_file"
    echo "" >> "$log_file"
    
    # Capture both output and execution time
    start_time=$(date +%s.%N)
    
    if [ "$RUN_GRID_SEARCH" = true ]; then
        $EXECUTABLE $GRID_SEARCH_ARGS $CUSTOM_PARAMS "$data_file" | tee -a "$log_file"
    else
        $EXECUTABLE "$data_file" | tee -a "$log_file"
    fi
    
    end_time=$(date +%s.%N)
    
    # Calculate execution time
    exec_time=$(echo "$end_time - $start_time" | bc)
    
    echo "" >> "$log_file"
    echo "=======================================================" >> "$log_file"
    echo "Total execution time: $exec_time seconds" >> "$log_file"
    echo "=======================================================" >> "$log_file"
    
    echo -e "${GREEN}Completed processing $data_file${NC}"
    echo ""
    
    ((COUNTER++))
done

echo -e "${GREEN}All data files processed. Results saved in logs directory.${NC}"

if [ "$RUN_GRID_SEARCH" = false ]; then
    echo -e "${BLUE}Summary of results:${NC}"

    # Display a summary of the best solutions
    echo -e "${YELLOW}File             | Best Algorithm    | Makespan${NC}"
    echo -e "${YELLOW}--------------------------------------------------------------------------------${NC}"

    for data_file in "${DATA_FILES[@]}"; do
        file_name=$(basename "$data_file")
        log_file="logs/${file_name%.txt}_results.log"
        
        # Extract the best solution from the log file
        best_line=$(grep -A20 "Overall best solution" "$log_file" | grep "Makespan:" | head -1)
        best_algorithm=$(grep -A5 "Overall best solution" "$log_file" | grep "Method:" | head -1 | awk -F": " '{print $2}')
        best_makespan=$(echo "$best_line" | awk '{print $2}')
        
        printf "${YELLOW}%-15s | %-17s | %s${NC}\n" "$file_name" "$best_algorithm" "$best_makespan"
    done
else
    echo -e "${BLUE}Grid search results are in the log files.${NC}"
fi

echo -e "${GREEN}Done!${NC}" 