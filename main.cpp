#include "GlfwCanvas.h"
#include <cairo/cairo.h>

const int WindowSize = 1024;

struct CairoContext {
  cairo_t *Context;
  unsigned char *Data;
  unsigned int Size;
  int Width, Height;

  ~CairoContext() {
    cairo_destroy(Context);
    delete Data;
    Context = NULL;
    Data = NULL;
  };
};

void drawCairo(GlfwCanvas &GlCanvas) {
  CairoContext Context;
  Context.Width = WindowSize;
  Context.Height = WindowSize;
  int Stride =
      cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, Context.Width);
  Context.Size = Stride * Context.Height;
  Context.Data = (unsigned char *)malloc(Context.Size);
  cairo_surface_t *Surface = cairo_image_surface_create_for_data(
      Context.Data, CAIRO_FORMAT_ARGB32, Context.Width, Context.Height, Stride);
  Context.Context = cairo_create(Surface);

  const float FontSize = 550.f;
  const float FontOffset = 126.f;
  cairo_set_source_rgba(Context.Context, 0, 0, .12, 1);
  cairo_paint(Context.Context);
  cairo_set_source_rgb(Context.Context, 1, 1, 1);
  cairo_set_font_size(Context.Context, FontSize);
  cairo_move_to(Context.Context, Context.Width * 0.5f - FontOffset,
                Context.Height * 0.5f + FontOffset);
  cairo_show_text(Context.Context, "a");

  GlCanvas.setFrameTexture(Context.Data, Context.Width, Context.Height);
}

int main(void) {
  GlfwCanvas Canvas;
  Canvas.initWindow(WindowSize, WindowSize);
  Canvas.setup();

  drawCairo(Canvas);

  while (Canvas.loop()) {
    glViewport(0, 0, Canvas.getWidth(), Canvas.getHeight());
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(Canvas.getProgram());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Canvas.getFrameTexture());

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  
  return 0;
}
