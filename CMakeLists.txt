cmake_minimum_required(VERSION 3.22)
project(
	Arena VERSION 0.0.1
	DESCRIPTION "An arena allocator."
	LANGUAGES C
	)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

if(${ASAN})
	if(${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_C_COMPILER_ID} MATCHES "GNU")
		add_compile_options("-g" "-fsanitize=address" "-fno-omit-frame-pointer")
		add_link_options("-fsanitize=address" "-fno-omit-frame-pointer")
	elseif(${CMAKE_C_COMPILER_ID} MATCHES "MSVC")
		add_compile_options("/Zi" "/fsanitize=address")
	endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(arena STATIC src/arena.c src/arena_string.c)
target_include_directories(arena PUBLIC "include")

add_executable(arena_test EXCLUDE_FROM_ALL src/main.c)
target_link_libraries(arena_test PRIVATE arena)

get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

foreach(current_target IN LISTS target_names)
	if(MSVC)
		target_compile_options(${current_target} PRIVATE /W3 /WX)
	else()
		target_compile_options(${current_target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endforeach()
