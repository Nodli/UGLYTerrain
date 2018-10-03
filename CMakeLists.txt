cmake_minimum_required (VERSION 3.5)

project (genTerrain)

set(CMAKE_CXX_STANDARD 11)

include_directories("src/include")

add_executable(genTerrain 
    "src/main.cpp")

