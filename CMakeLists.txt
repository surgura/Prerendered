cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(vulkan)

set(CMAKE_CXX_STANDARD 17)

# set location of module finding scrips
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

file(GLOB SRC
    "src/*.h"
    "src/*.cpp"
)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC})
#include_directories("C:/libraries/glm-0.9.9.2")

#target_link_libraries(main
#  core)

# find SFML module
find_package(SFML COMPONENTS system window graphics REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)