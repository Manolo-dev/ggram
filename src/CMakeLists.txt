# include all cpp files in the current directory
list(APPEND sources
	${CMAKE_CURRENT_LIST_DIR}/rules/parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/utils.cpp
	${CMAKE_CURRENT_LIST_DIR}/error/ggram_error.cpp
	${CMAKE_CURRENT_LIST_DIR}/error/file_error.cpp
	${CMAKE_CURRENT_LIST_DIR}/error/syntax_error.cpp
	${CMAKE_CURRENT_LIST_DIR}/error/argument_error.cpp
	${CMAKE_CURRENT_LIST_DIR}/input_handler.cpp
	${CMAKE_CURRENT_LIST_DIR}/file_handler.cpp
	${CMAKE_CURRENT_LIST_DIR}/lexme/lexme.cpp
	${CMAKE_CURRENT_LIST_DIR}/rules/rules.cpp
	${CMAKE_CURRENT_LIST_DIR}/rules/generator.cpp
)
