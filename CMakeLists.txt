cmake_minimum_required(VERSION 3.10)
project(PFSP_Metaheuristics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/problem.cpp
    src/solution.cpp
    src/metaheuristic.cpp
    src/constructive.cpp
    src/local_search.cpp
    src/iterated_local_search.cpp
    src/simulated_annealing.cpp
    src/tabu_search.cpp
    src/genetic_algorithm.cpp
    src/ant_colony.cpp
    src/grid_search.cpp
)

# Add header files
set(HEADERS
    include/problem.hpp
    include/solution.hpp
    include/metaheuristic.hpp
    include/constructive.hpp
    include/local_search.hpp
    include/iterated_local_search.hpp
    include/simulated_annealing.hpp
    include/tabu_search.hpp
    include/genetic_algorithm.hpp
    include/ant_colony.hpp
    include/grid_search.hpp
)

# Create executable
add_executable(pfsp ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(pfsp PRIVATE include)

# Add compiler warnings
if(MSVC)
    target_compile_options(pfsp PRIVATE /W4)
else()
    target_compile_options(pfsp PRIVATE -Wall -Wextra -Wpedantic)
endif() 