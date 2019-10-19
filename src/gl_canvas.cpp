#include "gl_canvas.h"
#include <stdio.h>
#include <stdlib.h>

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}
static void keyCallback(GLFWwindow *window, int key, int scan_code, int action,
                         int Mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLCanvas::GLCanvas() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

void GLCanvas::init_window(int width, int height,
                            const char *window_name) {
  window = glfwCreateWindow(width, height, window_name, NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);

  this->width = width;
  this->height = height;
}

bool GLCanvas::loop() {
  if (glfwWindowShouldClose(window)) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return false;
  }

  glfwSwapBuffers(window);
  glfwPollEvents();
  return true;
}

GLCanvas::~GLCanvas() {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void GLCanvas::setup() {
  compile_shaders();
  create_vertex_buffer();
}

void GLCanvas::set_frame_texture(unsigned char *data, int texture_width,
                                 int texture_height) {
  if (!texture_created) {
    glGenTextures(1, &frame_texture);

    texture_created = true;
  }
  glBindTexture(GL_TEXTURE_2D, frame_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, data);

  glUseProgram(program);
  glUniform1i(glGetUniformLocation(program, "Frame"), 0);
}

void GLCanvas::create_vertex_buffer() {
  const float vertices[12] = {-1.f, -1.f, /**/ -1.f, 1.f,  /**/ 1.f,  1.f, /**/
                              1.f,  1.f,  /**/ 1.f,  -1.f, /**/ -1.f, -1.f};

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint vertex_position_location = glGetAttribLocation(program, "Positions");

  glEnableVertexAttribArray(vertex_position_location);
  glVertexAttribPointer(vertex_position_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(float) * 2, (void *)0);
}

static const char *vertex_shader_str = "\
uniform mat4 MVP; \
attribute vec2 Positions; \
varying vec2 Uv; \
\
void main() {\
  gl_Position = vec4(Positions, 0.0, 1.0); \
  Uv = (Positions + vec2(1.0, 1.0)) * 0.5; \
  Uv = vec2(Uv.x, 1.0 - Uv.y); \
}\
";

static const char *fragment_shader_str = "\
varying vec2 Uv; \
uniform sampler2D Frame; \
void main() {\
  vec4 tex = texture2D(Frame, Uv); \n\
  gl_FragColor = tex.zyxw; \
}\
";

void GLCanvas::compile_shaders() {
  GLuint vertex_shader, fragment_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_str, NULL);
  glCompileShader(vertex_shader);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_str, NULL);
  glCompileShader(fragment_shader);
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
}
