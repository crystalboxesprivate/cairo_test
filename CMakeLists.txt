cmake_minimum_required(VERSION 3.12)
project(cairo_test)
set(MYROOTDIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(ext)
add_subdirectory(lib)

set(common_sources 
include/app.h
src/app.cpp

include/gl_canvas.h
src/gl_canvas.cpp
)

function (configure_graphics_target target_name source_name library_name) 
  add_executable(${target_name} ${source_name} ${common_sources})
  target_include_directories(${target_name} PUBLIC include)
  target_link_libraries(${target_name} PUBLIC glfw ${library_name} glad)
endfunction()

configure_graphics_target(cairo_test src/main_cairo.cpp cairo-static)
configure_graphics_target(skia_test src/main_skia.cpp skia)
