set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.0.0)
project(opengl_playground VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(include /usr/include/GL /usr/include/GLFW /usr/include/glm /usr/include/SOIL lib/CImg-2.4.2)
link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)
link_libraries(GLEW glfw GL SOIL)

file(GLOB opengl_playground_src "src/*.cpp")

add_executable(opengl_playground ${opengl_playground_src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)