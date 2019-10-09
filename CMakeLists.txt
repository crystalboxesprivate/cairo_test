cmake_minimum_required(VERSION 3.12)

add_subdirectory(lib)
add_subdirectory(ext)

project(cairo_test)
include_directories(include)

add_executable(cairo_test 
  include/Shaders.h
  include/GlfwCanvas.h
  include/GLStuff.h
  include/Misc.h
  src/GlfwCanvas.cpp
  src/main.cpp
)

target_link_libraries(cairo_test PUBLIC glfw cairo glad)
