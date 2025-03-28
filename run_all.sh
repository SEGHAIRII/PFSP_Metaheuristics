#!/bin/bash

# Colors for better readability
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
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

# Count data files
DATA_FILES=( $(find data -name "*.txt" | sort) )
TOTAL=${#DATA_FILES[@]}
COUNTER=1

# Process each data file
for data_file in "${DATA_FILES[@]}"; do
    file_name=$(basename "$data_file")
    log_file="logs/${file_name%.txt}_results.log"
    
    echo -e "${GREEN}[$COUNTER/$TOTAL] Processing $data_file${NC}"
    echo -e "${BLUE}Results will be saved to $log_file${NC}"
    
    # Run the program and save output to log file
    echo "=======================================================" > "$log_file"
    echo "Running metaheuristics on $file_name" >> "$log_file"
    echo "=======================================================" >> "$log_file"
    echo "" >> "$log_file"
    
    # Capture both output and execution time
    start_time=$(date +%s.%N)
    $EXECUTABLE "$data_file" | tee -a "$log_file"
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

echo -e "${GREEN}Done!${NC}" 