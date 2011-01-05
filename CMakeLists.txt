cmake_minimum_required (VERSION 2.6)

set (PROJECT SpiceExtract)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_UI *.ui)
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )


find_package (Qt4 REQUIRED)

include_directories(./)
QT4_WRAP_UI(${PROJECT}_UIS_H ${${PROJECT}_UI})

qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
add_library (${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_UIS_H})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")
	
	set (LINK 
	Core
	GUI
	Util
	${QT_USE_FILE}
	)
	
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(util)
target_link_libraries (${PROJECT} ${LINK} ${QT_LIBRARIES})



#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")


