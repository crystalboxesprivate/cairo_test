#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLCanvas {
public:
  GLCanvas();
  ~GLCanvas();
	inline GLuint get_program() { return program; }
  inline int get_width() { return width; }
  inline int get_height() { return height; }
  unsigned int get_frame_texture() { return frame_texture; }

  void init_window(int width, int height, const char *window_name = "Canvas");
  // set shaders
  void setup();
  bool loop();
  void destroy();
  void set_frame_texture(unsigned char *data, int texture_width,
                       int texture_height);

private:
  void create_vertex_buffer();
  void compile_shaders();

  GLFWwindow *window = NULL;
  GLuint program;
  int width, height;
  unsigned int frame_texture;
  bool texture_created = false;
};
