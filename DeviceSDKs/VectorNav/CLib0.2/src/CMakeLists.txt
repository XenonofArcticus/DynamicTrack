project(VNavLib)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(arch/win32/ SRC_LIST)
aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

