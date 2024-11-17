cmake_minimum_required(VERSION 3.10)  # Specify the minimum CMake version
project(lanc VERSION 1.0 LANGUAGES C)  # Define your project name and version, specify C language

set(CMAKE_C_STANDARD 11)  # Set the C standard (C11)

# Specify the source files in the src directory
file(GLOB SOURCES "src/*.c")  # This will include all .c files in the src directory

# Add the executable
add_executable(lanc ${SOURCES})  # Use the SOURCES variable to include all source files

# Specify include directories for lanc
target_include_directories(lanc PRIVATE includes)  # Include directories for the lanc target
