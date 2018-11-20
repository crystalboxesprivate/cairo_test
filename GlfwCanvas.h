#pragma once
#include "GLStuff.h"
#include "Misc.h"
#include "Shaders.h"

class GlfwCanvas {
public:
  GlfwCanvas() {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
      exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  }

  GLuint getProgram() { return Program; }
  int getWidth() { return Width; }
  int getHeight() { return Height; }
  unsigned int getFrameTexture() { return FrameTexture; }

  void initWindow(int Width, int Height, const char *WindowName = "Canvas") {
    Window = glfwCreateWindow(Width, Height, WindowName, NULL, NULL);
    if (!Window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(Window, keyCallback);
    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    this->Width = Width;
    this->Height = Height;
  }
  // set shaders
  void setup() {
    compileShaders();
    createVertexBuffer();
  }

  bool loop() {
    if (glfwWindowShouldClose(Window)) {
      glfwDestroyWindow(Window);
      glfwTerminate();
      return false;
    }

    glfwSwapBuffers(Window);
    glfwPollEvents();
    return true;
  }

  void destroy() {
    glfwDestroyWindow(Window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  }

  void setFrameTexture(unsigned char *Data, int TextureWidth,
                       int TextureHeight) {
    glGenTextures(1, &FrameTexture);
    glBindTexture(GL_TEXTURE_2D, FrameTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, Data);

    glUseProgram(Program);
    glUniform1i(glGetUniformLocation(Program, "Frame"), 0);
  }

private:
  void createVertexBuffer() {
    const float Vertices[12] = {
        -1.f, -1.f, /**/ -1.f, 1.f,  /**/ 1.f,  1.f, /**/
        1.f,  1.f,  /**/ 1.f,  -1.f, /**/ -1.f, -1.f};

    GLuint VertexBuffer;
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    GLuint VertexPositionLocation = glGetAttribLocation(Program, "Positions");

    glEnableVertexAttribArray(VertexPositionLocation);
    glVertexAttribPointer(VertexPositionLocation, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 2, (void *)0);
  }
  void compileShaders() {
    GLuint VertexShader, FragmentShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderText, NULL);
    glCompileShader(VertexShader);
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderText, NULL);
    glCompileShader(FragmentShader);
    Program = glCreateProgram();
    glAttachShader(Program, VertexShader);
    glAttachShader(Program, FragmentShader);
    glLinkProgram(Program);
  }

  GLFWwindow *Window = NULL;
  GLuint Program;
  int Width, Height;
  unsigned int FrameTexture;
};
