cmake_minimum_required(VERSION 3.12)
project(cairo_test)
set(MYROOTDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_VERBOSE_MAKEFILE on)

option(BUILD_CAIRO_TEST "Build cairo test" OFF)

if(EXISTS ${MYROOTDIR}/lib/skia)
  option(BUILD_SKIA_TEST "Build skia test" ON)
else()
  option(BUILD_SKIA_TEST "Build skia test" OFF)
endif()

if (BUILD_SKIA_TEST)
  option(USE_SKIA_SHARED_LIB "Use shared skia library" ON)
endif()

add_subdirectory(ext)
add_subdirectory(lib)

set(common_sources 
  include/app.h
  include/gl_canvas.h
  src/app.cpp
  src/gl_canvas.cpp
)

if (BUILD_CAIRO_TEST)
  add_executable(cairo_test src/main_cairo.cpp ${common_sources})
  target_include_directories(cairo_test PUBLIC include)
  target_link_libraries(cairo_test PUBLIC glfw cairo-static glad)
endif()


if (BUILD_SKIA_TEST)
  add_executable(skia_test src/main_skia.cpp ${common_sources})
  target_compile_definitions(skia_test PRIVATE SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

  if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "" OFF)
    set(SKIA_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/skia/lib/win)
    message(STATUS ${CMAKE_BUILD_TYPE})

    if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
      set(SKIA_LIB_DIR ${SKIA_LIB_DIR}/debug)
    else()
      set(SKIA_LIB_DIR ${SKIA_LIB_DIR}/release)
    endif()
    set(SKIA_LIBS 
      ${SKIA_LIBS}
      ${SKIA_LIB_DIR}/skia.lib
      ${SKIA_LIB_DIR}/sksg.lib
      ${SKIA_LIB_DIR}/skshaper.lib     
      ${SKIA_LIB_DIR}/skottie.lib      
      ${SKIA_LIB_DIR}/harfbuzz.lib     
      ${SKIA_LIB_DIR}/icu.lib
    )
  endif(MSVC)

  target_include_directories(skia_test PUBLIC include ext/skia)
  target_link_libraries(skia_test PUBLIC glfw glad)
  target_link_libraries(skia_test PRIVATE ${SKIA_LIBS})
  
  if (MSVC)
    unset(USE_MSVC_RUNTIME_LIBRARY_DLL CACHE)
  endif()
endif()

unset(BUILD_SKIA_TEST CACHE) 
unset(BUILD_CAIRO_TEST CACHE) 

message(STATUS ${CMAKE_EXE_LINKER_FLAGS_INIT})
