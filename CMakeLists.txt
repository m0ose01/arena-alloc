cmake_minimum_required(VERSION 3.22)
project(
	Arena VERSION 0.0.1
	DESCRIPTION "An arena allocator."
	LANGUAGES C
	)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(arena STATIC src/arena.c)
target_include_directories(arena PUBLIC "include")

add_executable(hello src/main.c)
target_link_libraries(hello arena)

get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

foreach(current_target IN LISTS target_names)
	if(MSVC)
		target_compile_options(${current_target} PRIVATE /W3)
	else()
		target_compile_options(${current_target} PRIVATE -Wall -Wextra -Wpedantic)
	endif()
endforeach()
