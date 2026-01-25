cmake_minimum_required(VERSION 3.26)
project(2bpp)

add_subdirectory(deps/glfw)

add_executable(2bpp 
  src/main.cpp
  src/glad.c
)

target_include_directories(2bpp PRIVATE
  include/
  deps/glfw/include
)

target_link_libraries(2bpp PRIVATE glfw)