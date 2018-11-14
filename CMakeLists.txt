cmake_minimum_required (VERSION 3.5)

project (genTerrain)

set(CMAKE_CXX_STANDARD 11)

include_directories("include"
                    "thirdParties"
                    "thirdParties/FastNoise"
                    "thirdParties/Eigen"
                    "thirdParties/ImGui"
                    ${GLFW_INCLUDE_DIRS})

set(imgui_sources
    "thirdParties/ImGui/imgui.cpp"
    "thirdParties/ImGui/imgui_widgets.cpp"
    "thirdParties/ImGui/imgui_draw.cpp"
    "thirdParties/ImGui/imgui_demo.cpp"
    "thirdParties/ImGui/imgui_impl_glfw.cpp"
    "thirdParties/ImGui/imgui_impl_opengl3.cpp")

set(fnoise_sources
    "thirdParties/FastNoise/FastNoise.cpp")

set(sources
    "src/Box2d.cpp"
    "src/Grid2d.cpp"
    "src/ScalarField.cpp"
    "src/MultiLayerMap.cpp"
    "src/Noise/ParameterizedNoise.cpp"
    "src/Noise/FieldValues.cpp"
    "src/Noise/TerrainNoise.cpp")

set(test_sources
    "src/tests/test_Box2d.cpp"
    "src/tests/test_Grid2d.cpp"
    "src/tests/test_ScalarField.cpp"
    "src/tests/test_FieldValues.cpp")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_library(terrain STATIC ${sources})
add_library(imgui STATIC ${imgui_sources})
target_link_libraries(imgui ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} glfw)
add_library(fnoise STATIC ${fnoise_sources})


add_executable(genTerrain
    "src/main.cpp")
target_link_libraries(genTerrain ${GLEW_LIBRARIES} glfw imgui fnoise terrain)

add_executable(tests
    ${test_sources})
target_link_libraries(tests fnoise terrain)
