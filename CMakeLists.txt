cmake_minimum_required(VERSION 3.12)
project(cairo_test)

set(MYROOTDIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(ext)
project(cairo_test)
add_subdirectory(lib)

add_executable(cairo_test 
  include/gl_canvas.h
  src/gl_canvas.cpp
  src/main.cpp
)

target_include_directories(cairo_test PUBLIC include)
target_link_libraries(cairo_test PUBLIC glfw cairo-static glad)
