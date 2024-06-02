cmake_minimum_required(VERSION 3.6.0)
set(CMAKE_CXX_STANDARD 17)
project(ignoreme)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/glad
	${PROJECT_SOURCE_DIR}/imgui
	${PROJECT_SOURCE_DIR}/imgui/backends
	${PROJECT_SOURCE_DIR}/imgui/misc/cpp
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
	"${PROJECT_SOURCE_DIR}/glad/*"
	"${PROJECT_SOURCE_DIR}/imgui/*"
	"${PROJECT_SOURCE_DIR}/imgui/backends*"
	#FIXME: The trailing '/' at the end of a line MATTERS! "backends" is not working correctly, but misc/cpp/ IS
	"${PROJECT_SOURCE_DIR}/imgui/misc/cpp/*"
)
add_definitions(-DGLES2)
add_executable(ignoreme WIN32 ${SRCS} "main.cpp")
#add_executable(ignoreme "src/main.cpp")
find_package(SDL2 REQUIRED)
target_link_libraries(ignoreme ${SDL2_LIBRARIES})
