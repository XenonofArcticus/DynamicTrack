project(DTTest)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
	VNavLib
	DTLibrary
	${Boost_SERIALIZATION_LIBRARY} ${Boost_TIMER_LIBRARY}
)

include_directories(${Boost_INCLUDE_DIRS} ../DeviceSDKs/VectorNav/CLib0.2/include ../Source)

if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} pthread)
endif()

