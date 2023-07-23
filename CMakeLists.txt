cmake_minimum_required(VERSION 3.10)
project(ignoreme)
add_executable(ignoreme main.cpp)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/pets
	${PROJECT_SOURCE_DIR}/glm
	#The above line *needs* to be here to use bracket includes <> for glm - and you HAVE To refresh CMakeLists in Visual Studio for intellisense to pick up the path!
)
