option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)

if (TRACY_ENABLE)
	add_compile_definitions(TRACY_ENABLE)
endif()

add_subdirectory(tracy)
