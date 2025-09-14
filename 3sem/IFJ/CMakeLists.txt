cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ifj)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Add debugging flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Add the executable and set the output name to 'app'
add_executable(ifjcompiler main.c tokeniser.c parser.c error.c psa.c prec_table.c expr_stack.c symtable.c codegen.c)
