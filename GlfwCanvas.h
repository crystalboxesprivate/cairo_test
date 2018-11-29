#pragma once
#include "GLStuff.h"

class GlfwCanvas {
public:
  GlfwCanvas();
  ~GlfwCanvas();
  GLuint getProgram() { return Program; }
  int getWidth() { return Width; }
  int getHeight() { return Height; }
  unsigned int getFrameTexture() { return FrameTexture; }

  void initWindow(int Width, int Height, const char *WindowName = "Canvas");
  // set shaders
  void setup();
  bool loop();
  void destroy();
  void setFrameTexture(unsigned char *Data, int TextureWidth,
                       int TextureHeight);

private:
  void createVertexBuffer();
  void compileShaders();

  GLFWwindow *Window = NULL;
  GLuint Program;
  int Width, Height;
  unsigned int FrameTexture;
  bool TextureCreated = false;
};
