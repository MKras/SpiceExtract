cmake_minimum_required (VERSION 2.6)

set (PROJECT Util)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )


include_directories(
	${CMAKE_SOURCE_DIR}
	${BUILD_ROOT}/util
	${CMAKE_SOURCE_DIR}/../core
	${BUILD_ROOT}/core
	)

qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
add_library (${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

add_dependencies (${PROJECT} Core)
target_link_libraries (${PROJECT} Core)

if(WITH_TEST)
add_subdirectory (tests)
endif(WITH_TEST)

#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")


