cmake_minimum_required (VERSION 2.6)

set (PROJECT SpiceExtract)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")


option (WITH_LEVMAR "Use LevMar library" OFF)
option (WITH_GALIB "Use GALib library" OFF)
option (WITH_OPTPP "Use OPT++ library" OFF)
option (USE_X86_64 "Use x86_64 arch" ON)


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
SET (QWT_DIR $ENV{QWT_DIR})
set (QWT_INCLUDE_DIR  /usr/local/include /usr/include/qwt5 /usr/include)
find_library (QWT_LIB libqwt.so  PATHS ${QWT_DIR} /usr/lib/ /usr/lib64/ /usr/local/lib/ /usr/local/lib64/)
if (NOT QWT_LIB)
message (FATAL_ERROR QWT_LIB=${QWT_LIB})
else ()
message (STATUS QWT_LIB=${QWT_LIB})
endif(NOT QWT_LIB)

################################################
#### LAPACK  #####################################
SET (LAPACK_DIR $ENV{LAPACK_DIR})
set (LAPACK_INCLUDE_DIR  ${LAPACK_DIR}/include /usr/local/include /usr/include/lapack /usr/include)
find_library (LAPACK_LIB liblapack.so  PATHS ${LAPACK_DIR} /usr/lib/ /usr/lib64/ /usr/local/lib/ /usr/local/lib64/)

if (NOT LAPACK_LIB)
message (FATAL_ERROR LAPACK_LIB=${LAPACK_LIB})
else ()
message (STATUS LAPACK_LIB=${LAPACK_LIB})
endif(NOT LAPACK_LIB)
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


find_file(LIBF2C libf2c.a PATHS /usr/lib/ /usr/lib64/ /usr/local/lib/ /usr/local/lib64/)
if (NOT LIBF2C)
message (FATAL_ERROR LIBF2C=${LIBF2C})
else()
message (STATUS LIBF2C=${LIBF2C})
endif(NOT LIBF2C)


set(LINK 	
	lapack
        ${LAPACK_LIB}
	gslcblas 
	gsl
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

target_link_libraries (${PROJECT} ${LINK} ${QT_LIBRARIES} ${LIBF2C} ${QWT_LIB})

set (DEFFINITIONS
	_REENTRANT
	QT_SHARED
	)
	
add_definitions( -c )	
if(USE_X86_64)
    message( STATUS "YOU use X86_64 arch!\n")
    add_definitions( -m64 )
endif(USE_X86_64)
add_definitions( -pipe )	
add_definitions( -g )	
add_definitions( -Wall )	
add_definitions( -W)	

set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

message( STATUS "Building ${PROJECT} finished\n")


