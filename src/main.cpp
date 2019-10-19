#include "gl_canvas.h"
#include <cairo/cairo.h>
#include <math.h>  
#include <stdlib.h>

const int WINDOW_SIZE = 1024;

class TimeManager {
public:
  void update() { frame_count++; }
  unsigned int get_frame_count() { return frame_count; }
  float get_seconds() { return frame_count / (float)frames_per_second; }

private:
  const int frames_per_second = 120;
  unsigned int frame_count = 0;
};

struct CairoContext {
  cairo_t *context;
  unsigned char *data;
  unsigned int size;
  int width, height;

  ~CairoContext() {
    cairo_destroy(context);
    delete data;
    context = NULL;
    data = NULL;
  };
};

void init_cairo_surface(CairoContext &ctx) {
  ctx.width = WINDOW_SIZE;
  ctx.height = WINDOW_SIZE;
  int stride =
      cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, ctx.width);
  ctx.size = stride * ctx.height;
  ctx.data = (unsigned char *)malloc(ctx.size);
  cairo_surface_t *surface = cairo_image_surface_create_for_data(
      ctx.data, CAIRO_FORMAT_ARGB32, ctx.width, ctx.height, stride);
  ctx.context = cairo_create(surface);
}

void draw_cairo(CairoContext &ctx, TimeManager &time) {
  // make a font letter scaling up and down
  cairo_set_source_rgba(ctx.context, 0, 0, .12, 1);
  cairo_paint(ctx.context);

  float size_mult = ((float)sin(time.get_seconds() * 8.232f) + 1.f) * .5f;

  const float font_size = 550.f + size_mult * 110.f;
  const float font_offset = 126.f;

  cairo_set_source_rgb(ctx.context, 1, 1, 1);
  cairo_set_font_size(ctx.context, font_size);
  cairo_move_to(ctx.context, ctx.width * 0.5f - font_offset,
                ctx.height * 0.5f + font_offset);
  cairo_show_text(ctx.context, "a");
}

int main(void) {
  TimeManager time;
  GLCanvas gl_canvas;
  gl_canvas.init_window(WINDOW_SIZE, WINDOW_SIZE);
  gl_canvas.setup();

  CairoContext ctx;
  init_cairo_surface(ctx);

  draw_cairo(ctx, time);

  while (gl_canvas.loop()) {
    time.update();
    glViewport(0, 0, gl_canvas.get_width(), gl_canvas.get_height());
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gl_canvas.get_program());
    glActiveTexture(GL_TEXTURE0);

    draw_cairo(ctx, time);
    gl_canvas.set_frame_texture(ctx.data, ctx.width, ctx.height);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  return 0;
}
