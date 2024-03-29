cmake_minimum_required(VERSION 3.20.0)

project(TicTacToe)

# Required C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

#Set binary and build directory
set(CMAKE_BINARY_DIR "${CMAKE_HOME_DIRECTORY}/bin")

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})

	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG})

endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# Download all the Submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")

	# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule Update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, Please checkout submodules")
		endif()
	endif()
endif()

# Check all the Submodules
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/dependencies/glfw/CMakeLists.txt")
	message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

find_package( OpenGL REQUIRED )

# Disable GLFW documentation
set(GLFW_BUILD_DOCS OFF
	CACHE BOOL "" FORCE
)

# Disable GLFW tests
set(GLFW_BUILD_TESTS OFF
	CACHE BOOL "" FORCE
)

# Disable GLFW examples
set(GLFW_BUILD_EXAMPLES OFF
	CACHE BOOL "" FORCE
)

add_subdirectory(
	dependencies/glad
)

add_subdirectory(
	dependencies/glfw
)

if( MSVC )
	SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
	set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
endif()

add_executable(${PROJECT_NAME}
	src/source.cpp
	src/game.cpp
	src/shader.cpp
	src/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	dependencies/include/
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE ${OPENGL_LIBRARIES}
	PRIVATE glad
	PRIVATE glfw
)
