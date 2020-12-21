cmake_minimum_required(VERSION 3.17)
project(day18)

set(CMAKE_CXX_STANDARD 20)
include_directories(/usr/local/include/antlr4-runtime)

set(antlr4-GENERATED_SRC
        ${PROJECT_SOURCE_DIR}/generated/ALexer.h
        ${PROJECT_SOURCE_DIR}/generated/ALexer.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParser.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParser.h
        ${PROJECT_SOURCE_DIR}/generated/AParserBaseVisitor.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParserBaseVisitor.h
        ${PROJECT_SOURCE_DIR}/generated/AParserVisitor.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParserVisitor.h
        ${PROJECT_SOURCE_DIR}/generated/AParser2.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParser2.h
        ${PROJECT_SOURCE_DIR}/generated/AParser2BaseVisitor.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParser2BaseVisitor.h
        ${PROJECT_SOURCE_DIR}/generated/AParser2Visitor.cpp
        ${PROJECT_SOURCE_DIR}/generated/AParser2Visitor.h
        )

set(project_src
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${antlr4-GENERATED_SRC} parser.h parser2.h)

add_executable(day18 ${project_src})
target_link_libraries(day18 /usr/local/lib/libantlr4-runtime.a)