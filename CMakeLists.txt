cmake_minimum_required(VERSION 3.12)

if(WIN32)
  project(cairo-opengl-canvas)
  include_directories("${PROJECT_SOURCE_DIR}/include")
  
  add_executable(cairo_ogl_canvas 
    glad.c
    Shaders.h
    
    GlfwCanvas.h
    GlfwCanvas.cpp

    GLStuff.h
    Misc.h
    main.cpp
  )

  target_link_libraries(cairo_ogl_canvas 
    "${PROJECT_SOURCE_DIR}/lib/x64/cairo.lib"
    "${PROJECT_SOURCE_DIR}/lib/x64/glfw3.lib"
  )
  set(cairo_dll "${PROJECT_SOURCE_DIR}/lib/x64/cairo.dll")
  
  add_custom_command(TARGET cairo_ogl_canvas POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${cairo_dll} $<TARGET_FILE_DIR:cairo_ogl_canvas>)
endif()
