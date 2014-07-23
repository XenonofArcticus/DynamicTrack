project(VNavLib)
cmake_minimum_required(VERSION 2.8)

if(WIN32 AND MSVC)
    aux_source_directory(arch/win32/ SRC_LIST)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    aux_source_directory(arch/linux/ SRC_LIST)
endif()

aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

INSTALL(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

