#include <stdio.h>
#include <stdlib.h>

static void errorCallback(int Error, const char *Description) {
  fprintf(stderr, "Error: %s\n", Description);
}
static void keyCallback(GLFWwindow *Window, int Key, int ScanCode, int Action,
                         int Mods) {
  if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
    glfwSetWindowShouldClose(Window, GLFW_TRUE);
}