cmake_minimum_required (VERSION 2.6)

set (PROJECT SpiceExtract)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_UI *.ui)
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )

#### QWT  #####################################
set (QWT_INCLUDE_DIR  /usr/local/include)
set (QWT_DIR  /usr/local/)
set (QWT_LIB ${QWT_DIR}/lib/libqwt.so.5.2.0)
################################################
SET (BUILD_ROOT $ENV{BUILD_ROOT})

########### Temperary!!!!!!!!!!!!!
#SET (BUILD_ROOT ./)

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
QT4_WRAP_UI(${PROJECT}_UIS_H ${${PROJECT}_UI})

qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
add_executable (${PROJECT} ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_UIS_H})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")
	
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(util)

set(LINK 
	Core
	GUI
	Util

	lapack
	gslcblas 
	gsl
	levmar
	newton
	ga
	newmat
	opt
	/usr/lib/libf2c.a
	${QWT_LIB}
	)

#message( STATUS "\n\n!LINK = ${LINK}")

target_link_libraries (${PROJECT} ${LINK} ${QT_LIBRARIES})

set (DEFFINITIONS
	QT_SHARED
	)
set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")

#stance, to enable OpenGL support add the following line to your CMakeLists.txt.

#ADD_DEFINITIONS( -Wall )
# INCLUDE( ${QT_USE_FILE} )


#SET(QT_USE_QTOPENGL TRUE)

#The most commonly used modules are controlled using the following variables.

   # * QT_USE_QTNETWORK
    #* QT_USE_QTOPENGL
    #* QT_USE_QTSQL
    #* QT_USE_QTXML
    #* QT_USE_QTSVG
   # * QT_USE_QTTEST
   # * QT_USE_QTDBUS
    #* QT_USE_QTSCRIPT
   # * QT_USE_QTWEBKIT
   # * QT_USE_QTXMLPATTERNS
   # * QT_USE_PHONON

