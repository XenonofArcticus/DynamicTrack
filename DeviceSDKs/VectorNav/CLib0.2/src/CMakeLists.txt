project(VNavLib)
cmake_minimum_required(VERSION 2.8)

if(WIN32 AND MSVC)
    aux_source_directory(arch/win32/ SRC_LIST)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    aux_source_directory(arch/linux/ SRC_LIST)
endif()

aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lpthread)

INSTALL(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

SET(HEADER_FILES
	vectornav.h
	vn100.h
	vn200.h
	vn_common.h
	vncp_services.h
	vn_errorCodes.h
	vn_kinematics.h
	vn_linearAlgebra.h
	vn_math.h
)

FOREACH(INCLUDEFILE ${HEADER_FILES})
	# FILE(RELATIVE_PATH REL_INCLUDEFILE ${VNavLib_SOURCE_DIR}/../include ${INCLUDEFILE})
	# GET_FILENAME_COMPONENT(REL_INCLUDE_PATH ${REL_INCLUDEFILE} PATH)
	INSTALL(
		FILES ${VNavLib_SOURCE_DIR}/../include/${INCLUDEFILE}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/include/vectornav/
	)
ENDFOREACH(INCLUDEFILE)

