project(DTLibrary)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} pthread)
endif()

