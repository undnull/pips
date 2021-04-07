cmake_minimum_required(VERSION 3.10)
project(pips VERSION 0.0.0)

option(PIPS_BUILD_EXAMPLES "Build examples" ON)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pips STATIC "${CMAKE_CURRENT_LIST_DIR}/pips.c")
target_include_directories(pips PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

include(CheckIncludeFile)
check_include_file(unistd.h HAVE_UNISTD)
if(NOT HAVE_UNISTD)
    # imagine using slow stdout for a unix-only
    # console application. ugh!
    message(FATAL_ERROR "unistd.h precense is required")
endif()

if(PIPS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
