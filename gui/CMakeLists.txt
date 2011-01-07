cmake_minimum_required (VERSION 2.6)

set (PROJECT GUI)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_UI *.ui )
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )


include_directories(
	${CMAKE_SOURCE_DIR}
        ${BUILD_ROOT}/gui
	)
#add_executable(${PROJECT} ${${PROJECT}_SOURCE_LIST})

#set(${PROJECT}_UIS ${${PROJECT}_UI})
QT4_WRAP_UI(${PROJECT}_UIS_H ${${PROJECT}_UI})

qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
add_library (${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_UIS_H})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")


#target_link_libraries (${PROJECT} ${LINK})



#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")


