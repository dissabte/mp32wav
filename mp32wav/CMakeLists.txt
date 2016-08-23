cmake_minimum_required(VERSION 3.0)

project(mp32wav)

message(STATUS "Processing ${PROJECT_NAME}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(UNIX)
	add_definitions(-Wall -Werror)
endif(UNIX)

file(GLOB_RECURSE INCLUDE_FILES "src/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

set(APP_SOURCES ${INCLUDE_FILES} ${SOURCE_FILES})

if(NOT LAME_FOUND)
	include(../external/lame.cmake)
endif()

if(NOT UNITTEST_CPP_FOUND)
	include(../external/unittest-cpp.cmake)
endif()

add_executable(${PROJECT_NAME} ${APP_SOURCES})

add_dependencies(${PROJECT_NAME} mp3lame)
if(UNITTEST_CPP_FOUND)
	add_dependencies(${PROJECT_NAME} UnitTest++)
	add_subdirectory("tests")
endif()

target_link_libraries(${PROJECT_NAME} mp3lame)

message(STATUS "Processing ${PROJECT_NAME} is done")
