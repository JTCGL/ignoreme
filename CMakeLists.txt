cmake_minimum_required(VERSION 3.10)
project(ignoreme)
add_executable(ignoreme main.cpp)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/glm
)
