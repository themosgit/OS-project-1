# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProgram)

# Specify C standard
set(CMAKE_C_STANDARD 99)

# Set the compiler flags (like -Wall)
set(CMAKE_C_FLAGS "-Wall")

# Include directories (add your include directory path)
include_directories(${PROJECT_SOURCE_DIR}/miris_src/include)

# Source files
set(SOURCE_FILES
        miris_src/modules/main.c
        miris_src/modules/node.c
        miris_src/modules/prompt.c
        miris_src/modules/prompt.c
)

# Add the executable (program) target
add_executable(program ${SOURCE_FILES})

# Define where to output the compiled program (optional)
set_target_properties(program PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
