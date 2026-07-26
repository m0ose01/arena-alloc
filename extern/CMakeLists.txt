cmake_minimum_required(VERSION 4.1.2)

if (TRACY_ENABLE)
	option(TRACY_ENABLE "" ON)
	option(TRACY_ON_DEMAND "" ON)
	add_compile_definitions(TRACY_ENABLE)
else()
	option(TRACY_ENABLE, "" OFF)
	option(TRACY_ON_DEMAND "" OFF)
endif()

add_subdirectory(tracy)
