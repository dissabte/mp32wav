cmake_minimum_required(VERSION 3.0)

project(UnitTests)

message(STATUS "Processing ${PROJECT_NAME}...")

include_directories(${UNITTEST_CPP_DIR})

if(UNIX)
	add_definitions(-Wno-write-strings)
endif(UNIX)


file(GLOB TESTS_INCLUDE_FILES "*.h")
file(GLOB TESTS_SOURCE_FILES "*.cpp")

# application files without the main.cpp
set(APP_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../src")
file(GLOB_RECURSE APP_INCLUDE_FILES "${APP_SOURCE_PATH}/*.h")
file(GLOB_RECURSE APP_SOURCE_FILES "${APP_SOURCE_PATH}/*.cpp")
list(REMOVE_ITEM APP_SOURCE_FILES "${APP_SOURCE_PATH}/main.cpp")

set(TESTS_SOURCES ${TESTS_INCLUDE_FILES} ${TESTS_SOURCE_FILES} ${APP_INCLUDE_FILES} ${APP_SOURCE_FILES})

add_executable(${PROJECT_NAME} ${TESTS_SOURCES})

target_link_libraries(${PROJECT_NAME} UnitTest++)

message(STATUS "Processing ${PROJECT_NAME} done")
