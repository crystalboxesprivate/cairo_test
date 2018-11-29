#include "GlfwCanvas.h"
#include <cairo/cairo.h>
#include <math.h>  

const int WindowSize = 1024;

class TimeManager {
public:
  void update() { FrameCount++; }
  unsigned int getFrameCount() { return FrameCount; }
  float getSeconds() { return FrameCount / (float)FramesPerSecond; }

private:
  const int FramesPerSecond = 120;
  unsigned int FrameCount = 0;
};

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

void initCairoSurface(CairoContext &Context) {
  Context.Width = WindowSize;
  Context.Height = WindowSize;
  int Stride =
      cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, Context.Width);
  Context.Size = Stride * Context.Height;
  Context.Data = (unsigned char *)malloc(Context.Size);
  cairo_surface_t *Surface = cairo_image_surface_create_for_data(
      Context.Data, CAIRO_FORMAT_ARGB32, Context.Width, Context.Height, Stride);
  Context.Context = cairo_create(Surface);
}

void drawCairo(CairoContext &Context, TimeManager &Time) {
  // make a font letter scaling up and down
  cairo_set_source_rgba(Context.Context, 0, 0, .12, 1);
  cairo_paint(Context.Context);

  float SizeMultiplier = ((float)sin(Time.getSeconds() * 8.232f) + 1.f) * .5f;

  const float FontSize = 550.f + SizeMultiplier * 110.f;
  const float FontOffset = 126.f;

  cairo_set_source_rgb(Context.Context, 1, 1, 1);
  cairo_set_font_size(Context.Context, FontSize);
  cairo_move_to(Context.Context, Context.Width * 0.5f - FontOffset,
                Context.Height * 0.5f + FontOffset);
  cairo_show_text(Context.Context, "a");
}

int main(void) {
  TimeManager Time;
  GlfwCanvas Canvas;
  Canvas.initWindow(WindowSize, WindowSize);
  Canvas.setup();

  CairoContext Context;
  initCairoSurface(Context);

  drawCairo(Context, Time);

  while (Canvas.loop()) {
    Time.update();
    glViewport(0, 0, Canvas.getWidth(), Canvas.getHeight());
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(Canvas.getProgram());
    glActiveTexture(GL_TEXTURE0);

    drawCairo(Context, Time);
    Canvas.setFrameTexture(Context.Data, Context.Width, Context.Height);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  return 0;
}
