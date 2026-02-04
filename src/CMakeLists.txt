option(BUILD_APP "Build the ray tracer application" ON)

if(BUILD_APP)
	add_executable(ray ray.c)

	# Expose vendored include paths so headers like <fix16.h> are found
	target_include_directories(ray PRIVATE
		${CMAKE_SOURCE_DIR}/vendored/libfixmatrix
		${CMAKE_SOURCE_DIR}/vendored/libfixmath
		${CMAKE_SOURCE_DIR}/include
	)

	# Link to vendored libraries if available
	if(TARGET fixmatrix)
		target_link_libraries(ray PRIVATE fixmatrix fixmath SDL3::SDL3)
	endif()
else()
	message(STATUS "BUILD_APP is OFF — not building 'ray' executable")
endif()