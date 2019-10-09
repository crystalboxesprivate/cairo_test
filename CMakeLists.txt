cmake_minimum_required(VERSION 3.12)
project(cairo_test)

set(MYROOTDIR ${CMAKE_CURRENT_SOURCE_DIR})
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

target_link_libraries(cairo_test PUBLIC glfw cairo-static glad)
