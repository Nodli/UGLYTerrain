cmake_minimum_required (VERSION 3.5)

project (genTerrain)

set(CMAKE_CXX_STANDARD 11)

include_directories("src/include")
include_directories("src/FastNoise")

set(sources
    "src/Box2d.cpp"
    "src/Grid2d.cpp"
    "src/ScalarField.cpp"
    "src/MultiLayerMap.cpp"
    "src/FastNoise/FastNoise.cpp")

set(test_sources
    "src/tests/test_Box2d.cpp"
    "src/tests/test_Grid2d.cpp"
    "src/tests/test_ScalarField.cpp")

add_library(terrain STATIC ${sources})

add_executable(genTerrain
    "src/main.cpp")
target_link_libraries(genTerrain terrain)

add_executable(tests
    ${test_sources})
target_link_libraries(tests terrain)
