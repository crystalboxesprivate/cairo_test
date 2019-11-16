#include <cairo/cairo.h>
#include <math.h>  
#include <stdlib.h>
#include "app.h"

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

class CairoApp : public GLApp {
public:
	virtual void init() {
		GLApp::init();
		ctx.width = get_width();
		ctx.height = get_height();
		int stride =
			cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, ctx.width);
		ctx.size = stride * ctx.height;
		ctx.data = (unsigned char*)malloc(ctx.size);
		cairo_surface_t* surface = cairo_image_surface_create_for_data(
			ctx.data, CAIRO_FORMAT_ARGB32, ctx.width, ctx.height, stride);
		ctx.context = cairo_create(surface);
	}

	virtual uint8_t* get_texture_data() { return ctx.data; }

	virtual void render_texture() override {
		cairo_set_source_rgba(ctx.context, 0, 0, .12, 1);
		cairo_paint(ctx.context);

		float size_mult = ((float)sin(time.get_seconds() * 8.232f) + 1.f) * .5f;

		const float font_size = 550.f + size_mult * 110.f;
		const float font_offset = 126.f;

		cairo_set_source_rgb(ctx.context, 1, 1, 1);
		cairo_set_font_size(ctx.context, font_size);
		cairo_move_to(ctx.context, get_width() * 0.5f - font_offset,
			get_height() * 0.5f + font_offset);
		cairo_show_text(ctx.context, "a");
	}

private:
	CairoContext ctx;
};

int main(void) {
	const int WINDOW_SIZE = 1024;

	CairoApp app;
	app.set_dimensions(WINDOW_SIZE, WINDOW_SIZE);
	app.init();
	app.run();
  return 0;
}

