cmake_minimum_required(VERSION 3.0.0)
project(ggram 
	LANGUAGES CXX 
	VERSION 0.1.0
	DESCRIPTION "Ggram is a lexer-parser generator. It translate a language clost to BNF into a lexer-parser written in cpp."
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
	-Wall -Wextra -Wpedantic -Werror
)

include("${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt")

add_executable(ggram src/main.cpp ${sources})


target_include_directories(ggram 
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
	PUBLIC "${PROJECT_BINARY_DIR}"
)
configure_file("${CMAKE_CURRENT_LIST_DIR}/include/version.hpp.in" version.hpp)
