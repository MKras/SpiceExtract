cmake_minimum_required (VERSION 2.6)

set (PROJECT SpiceExtract)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")


option (WITH_LEVMAR "Use LevMar library" OFF)
option (WITH_GALIB "Use GALib library" OFF)
option (WITH_OPTPP "Use OPT++ library" OFF)


file(GLOB ${PROJECT}_SOURCES *.cpp  
	util/*.cpp 
	core/*.cpp 
	gui/*.cpp)  
file(GLOB ${PROJECT}_HEADERS *.h *.hpp 
	util/*.h 
	core/*.h 
	gui/*.h)
file(GLOB ${PROJECT}_UI *.ui 
	util/*.ui 
	gui/*.ui)
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )

#### QWT  #####################################
set (QWT_INCLUDE_DIR  /usr/local/include)
set (QWT_DIR  /usr/local/)
set (QWT_LIB ${QWT_DIR}/lib/libqwt.so.5.2.0)
################################################
SET (BUILD_ROOT $ENV{BUILD_ROOT})

########### Temperary!!!!!!!!!!!!!
SET (DEBUG ON)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTGUI TRUE)
SET(QT_USE_QTCORE TRUE)

#-DQT_SCRIPT_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED 
find_package (Qt4 REQUIRED)

include( ${QT_USE_FILE} )

message(STATUS "ENV{BUILD_ROOT} = $ENV{BUILD_ROOT}")
set (INCLUDES 
	${CMAKE_SOURCE_DIR}
	${QWT_INCLUDE_DIR}
	${QT_INCLUDE_DIR} 
	${QT_QTCORE_INCLUDE_DIR}
	${QT_QTGUI_INCLUDE_DIR}
	/usr/include	
	${QWT_INCLUDE_DIR}
	${BUILD_ROOT}
)

include_directories(
	${INCLUDES}	
	)
	
set (RESOURCE_PATH 
	${CMAKE_SOURCE_DIR}/gui
	)
 
set (RESOURCES 
	${RESOURCE_PATH}/textedit.qrc)


qt4_add_resources (${PROJECT}_RESOURCES  ${RESOURCES})
qt4_wrap_UI(${PROJECT}_UIS_H ${${PROJECT}_UI})
qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})

#add_subdirectory(util)
#add_subdirectory(core)
#add_subdirectory(gui)


#add_dependencies (${PROJECT} Core GUI Util)
find_file(LIBF2C libf2c.a PATHS /usr/lib/)

set(LINK 
	#Core
	#Util	
	#GUI
	lapack
	gslcblas 
	gsl
        #levmar

        #ga
        #newton
        #newmat
        #opt
	#/usr/lib/libf2c.a
	${QWT_LIB}
	)
if(WITH_LEVMAR)
    add_definitions( -DWITH_LEVMAR)
    list (APPEND LINK levmar)
endif(WITH_LEVMAR)
if(WITH_GALIB)
    add_definitions( -DWITH_GALIB )
    list (APPEND LINK ga)
endif(WITH_LEVMAR)
if(WITH_OPTPP)
    add_definitions( -DWITH_OPTPP )
    list (APPEND LINK newton newmat opt)
endif(WITH_OPTPP)


add_executable (${PROJECT} ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_UIS_H} ${${PROJECT}_RESOURCES})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")
#add_dependencies (${PROJECT} Core GUI Util)	
#message( STATUS "\n\n!LINK = ${LINK}")

target_link_libraries (${PROJECT} ${LINK} ${QT_LIBRARIES} ${LIBF2C})

set (DEFFINITIONS
	#CMAKE_C_COMPILER="/usr/bin/g++" 
	#CMAKE_CXX_COMPILER="g++"
	_REENTRANT
	QT_SHARED
	)
	
add_definitions( -c )	
add_definitions( -m64 )	
add_definitions( -pipe )	
add_definitions( -g )	
add_definitions( -Wall )	
add_definitions( -W)	

set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")

#stance, to enable OpenGL support add the following line to your CMakeLists.txt.

#ADD_DEFINITIONS( -Wall )
# INCLUDE( ${QT_USE_FILE} )
#-c -m64 -pipe -g -Wall -W -D_REENTRANT -DQT_SCRIPT_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED

#SET(QT_USE_QTOPENGL TRUE)

#The most commonly used modules are controlled using the following variables.

