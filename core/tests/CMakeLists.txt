cmake_minimum_required (VERSION 2.6)

set (PROJECT Core_test)

project (${PROJECT})

message( STATUS "\nBuilding ${PROJECT} started")

file(GLOB ${PROJECT}_SOURCES *.cpp ) 
file(GLOB ${PROJECT}_HEADERS *.h *.hpp )
file(GLOB ${PROJECT}_SOURCE_LIST ${${PROJECT}_SOURCES} ${${PROJECT}_HEADERS} )

include_directories(
	${CMAKE_SOURCE_DIR}
	${BUILD_ROOT}/core
	)
#add_executable(${PROJECT} ${${PROJECT}_SOURCE_LIST})

#qt4_wrap_cpp (${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
#add_library (${PROJECT} STATIC ${${PROJECT}_SOURCE_LIST} ${${PROJECT}_MOC_SOURCES})	
#set_target_properties(${PROJECT}  PROPERTIES COMPILE_DEFINITIONS "${DEFFINITIONS}")

	
#target_link_libraries (${PROJECT} ${LINK})

cmake_minimum_required(VERSION 2.8)
find_package(Boost COMPONENTS system filesystem REQUIRED)


find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
...
target_link_libraries(testTheTester
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )


add_excecutable(testTheTester boostTester.cpp)
target_link_libraries(testTheTester ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
add_test(tester tester)





#enable_testing ()
message( STATUS "Building ${PROJECT} finished\n")


