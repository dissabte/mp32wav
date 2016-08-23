cmake_minimum_required(VERSION 3.0)

set(LAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../external/lame/")

if(EXISTS ${LAME_ROOT}/CMakeLists.txt)
	set(LAME_FOUND ON)
	add_subdirectory(${LAME_ROOT} lame)
else()
	message(FATAL_ERROR "Couldn't found lame's cmake config. Please update lame submodule.")
endif()
