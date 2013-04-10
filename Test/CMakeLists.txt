project(Test)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE DEBUG)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all libraries.")
MAKE_DIRECTORY(${LIBRARY_OUTPUT_PATH})
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables.")
MAKE_DIRECTORY(${EXECUTABLE_OUTPUT_PATH})
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH)

if(WIN32 AND MSVC)
    OPTION(BUILD_WIN32_USE_MP "Build with multiple processes (/MP option; VS 2005 and above)." ON)
    MARK_AS_ADVANCED( BUILD_WIN32_USE_MP)
    IF(BUILD_WIN32_USE_MP)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    ENDIF(BUILD_WIN32_USE_MP)
endif()

SET(CMAKE_DEBUG_POSTFIX d)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS serialization timer REQUIRED)

link_directories(${LIBRARY_OUTPUT_PATH})

aux_source_directory(. SRC_LIST)

include_directories(${Boost_INCLUDE_DIRS} ../DeviceSDKs/VectorNav/CLib0.2/include ../Source)

add_executable(DTTest ${SRC_LIST})
target_link_libraries(DTTest DTLibraryd VNavLibd ${Boost_SERIALIZATION_LIBRARY} ${Boost_TIMER_LIBRARY})


