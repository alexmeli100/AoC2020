cmake_minimum_required(VERSION 3.17)
project(day8)

set(CMAKE_CXX_STANDARD 20)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(day8 main.cpp)
target_link_libraries(day8 ${CONAN_LIBS})