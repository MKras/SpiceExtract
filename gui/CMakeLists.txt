cmake_minimum_required (VERSION 2.6)

set (PROJECT GUI)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )


include_directories(./)
#add_executable(${PROJECT} ${${PROJECT}_SOURCE_LIST})
add_library (${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST})
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

	
#target_link_libraries (${PROJECT} ${LINK})



#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")


