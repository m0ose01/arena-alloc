cmake_minimum_required(VERSION 4.1.2)

include(FetchContent)

FetchContent_Declare(
	tracy
	GIT_REPOSITORY "https://github.com/wolfpld/tracy.git"
	GIT_TAG "05cceee0df3b8d7c6fa87e9638af311dbabc63cb"
)

FetchContent_MakeAvailable(tracy)

if (TRACY_ENABLE)
	option(TRACY_ENABLE "" ON)
	option(TRACY_ON_DEMAND "" ON)
	add_compile_definitions(TRACY_ENABLE)
else()
	option(TRACY_ENABLE, "" OFF)
	option(TRACY_ON_DEMAND "" OFF)
endif()
