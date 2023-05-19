cmake_minimum_required(VERSION 3.0.0)
project(ggram 
	LANGUAGES CXX 
	VERSION 0.1.0
	DESCRIPTION "Ggram is a lexer-parser generator. It translate a language clost to BNF into a lexer-parser written in cpp."
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# clang-tidy support
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
	add_custom_target(
		tidy
		COMMAND ${CLANG_TIDY_EXE} "${PROJECT_SOURCE_DIR}/src/*.cpp" --fix -p "${PROJECT_SOURCE_DIR}" -fix-errors
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)
else()
	message(STATUS "clang-tidy not found.")
endif()

# clang-format support
find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(CLANG_FORMAT_EXE)
	add_custom_target(
		format
		COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/include/*/*.hpp
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
else()
	message(STATUS "clang-format not found.")
endif()
