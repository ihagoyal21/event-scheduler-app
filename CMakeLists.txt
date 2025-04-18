cmake_minimum_required(VERSION 3.10)
project(event_scheduler_cpp)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
set(SRC_FILES
    main.cpp
    scheduler.cpp
    dsa/graph_coloring.cpp
    dsa/backtracking.cpp
    dsa/greedy_scheduler.cpp
    dsa/priority_queue.cpp
    dsa/segment_tree.cpp
)

# Add executable
add_executable(event_scheduler_cpp ${SRC_FILES})

# Include directories for headers
target_include_directories(event_scheduler_cpp PRIVATE
    third_party
    .
    dsa
)

# Optional: Show all warnings (recommended for development)
if(MSVC)
    target_compile_options(event_scheduler_cpp PRIVATE /W4)
else()
    target_compile_options(event_scheduler_cpp PRIVATE -Wall -Wextra -pedantic)
endif()

# Optional: Link Boost if you use it (Crow requires Boost headers only, not linking)
# find_package(Boost REQUIRED)
# target_include_directories(event_scheduler_cpp PRIVATE ${Boost_INCLUDE_DIRS})

# Optional: Set output directory
set_target_properties(event_scheduler_cpp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Optional: Install target (for 'make install')
install(TARGETS event_scheduler_cpp DESTINATION bin)
