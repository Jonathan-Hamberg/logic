cmake_minimum_required(VERSION 3.3)
project(Logic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.cpp
    src/Equation.cpp)

add_executable(logic ${SOURCE_FILES})

set_target_properties(logic PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${CMAKE_SOURCE_DIR}/bin/
    RUNTIME_OUTPUT_DIRECTORY_RELEASE    ${CMAKE_SOURCE_DIR}/bin/)
