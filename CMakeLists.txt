cmake_minimum_required(VERSION 3.17)

project(string_operations)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(./src/backend)
add_subdirectory(./src/datastructure)
add_subdirectory(./src/frontend)

add_executable(${PROJECT_NAME} ./src/main.cpp)

target_link_libraries(${PROJECT_NAME} frontend)
target_link_libraries(${PROJECT_NAME} datastructure)
target_link_libraries(${PROJECT_NAME} backend)