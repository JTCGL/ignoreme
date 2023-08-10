cmake_minimum_required(VERSION 3.6.0)
project(ignoreme)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/glad
	${PROJECT_SOURCE_DIR}/imgui
	${PROJECT_SOURCE_DIR}/imgui/backends
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
	"${PROJECT_SOURCE_DIR}/glad/*"
	"${PROJECT_SOURCE_DIR}/imgui/*"
	"${PROJECT_SOURCE_DIR}/imgui/backends*"
)
add_definitions(-DGLES2)
add_executable(ignoreme WIN32 ${SRCS} "main.cpp")
#add_executable(ignoreme "src/main.cpp")
find_package(SDL2 REQUIRED)
target_link_libraries(ignoreme ${SDL2_LIBRARIES})
