cmake_minimum_required(VERSION 3.17)
project(day9)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(day9 main.cpp)
target_link_libraries(day9 ${CONAN_LIBS})