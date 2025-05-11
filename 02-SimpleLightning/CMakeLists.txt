# CMakeList.txt: 01-FirstWindow 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("02-SimpleLightning")

set(glfw3_DIR "D:\\Lib\\glfw\\install\\lib\\cmake\\glfw")
find_package(glfw3 REQUIRED)

# 引用
include_directories(
	"./"
	"./include"
	"../include"
	"${GLFW3_INCLUDE_DIR}"
)

# 将源代码添加到此项目的可执行文件。
add_executable (02-SimpleLightning
	"main.cpp" 
	"glad.c"
	"Shader.cpp"
	"Observer.cpp")

# TODO: 如有需要，请添加测试并安装目标。
target_link_libraries( 02-SimpleLightning
	"${GLFW3_LIBRARY}"
	opengl32
)