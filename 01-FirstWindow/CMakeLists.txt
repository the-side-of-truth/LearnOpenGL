# CMakeList.txt: 01-FirstWindow 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("01-FirstWindow")

set(glfw3_DIR "D:\\Lib\\glfw\\install\\lib\\cmake\\glfw")
find_package(glfw3 REQUIRED)

# 引用
include_directories(
	"D:\\Lib\\glad\\include"
	"${GLFW3_INCLUDE_DIR}"
)

# 将源代码添加到此项目的可执行文件。
add_executable (01-FirstWindow 
	"01-FirstWindow.cpp" 
	"01-FirstWindow.h"
	"glad.c"
	)

# TODO: 如有需要，请添加测试并安装目标。
target_link_libraries( 01-FirstWindow
	"${GLFW3_LIBRARY}"
	opengl32
)