#include "gl_canvas.h"

class TimeManager {
public:
	void update() { frame_count++; }
	unsigned int get_frame_count() { return frame_count; }
	float get_seconds() { return frame_count / (float)frames_per_second; }

private:
	const int frames_per_second = 120;
	unsigned int frame_count = 0;
};

class App {
public:
	virtual void init() = 0;
	virtual void render_loop() = 0;
	virtual void run() = 0;
};

class GLApp : public App {
public:
	int get_width() { return width; }
	int get_height() {
		return height;
	}
	virtual void render_texture() = 0;
	virtual uint8_t* get_texture_data() = 0;

	void set_dimensions(int new_width, int new_height) {
		width = new_width;
		height = new_height;
	}

	virtual void init() override {
		gl_canvas.init_window(get_width(), get_height());
		gl_canvas.setup();
	}

	virtual void render_loop() override {
		auto& app = *this;
		app.time.update();
		glViewport(0, 0, app.gl_canvas.get_width(), app.gl_canvas.get_height());
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(app.gl_canvas.get_program());
		glActiveTexture(GL_TEXTURE0);

		app.render_texture();
		app.gl_canvas.set_frame_texture(get_texture_data(), get_width(), get_height());

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual void run() override {
		while (gl_canvas.loop()) {
			render_loop();
		}
	}

	GLCanvas gl_canvas;
	TimeManager time;

private:
	int width;
	int height;
};
