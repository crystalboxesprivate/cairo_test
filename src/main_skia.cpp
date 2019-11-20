#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "app.h"
// 
#include "modules/skottie/include/Skottie.h"
//
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class SkiaApp : public GLApp {
public:
	virtual void init() {
		GLApp::init();

		SkGraphics::Init();
		info = SkImageInfo::Make(get_width(), get_height(), SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kOpaque_SkAlphaType);
		surface = SkSurface::MakeRaster(info);
		data.resize(4 * get_width() * get_height());

		std::ifstream file(std::string(SOURCE_DIR) + "/" + "travel-icons.json");
		std::stringstream buffer;
		buffer << file.rdbuf();
		json_data = buffer.str();

		skottie::Animation::Builder builder;
		animation = builder.make(json_data.c_str(), json_data.length());

		int a = 2;
	}

	virtual uint8_t* get_texture_data() { return data.data(); }

	virtual void render_texture() override {
		auto canvas = surface->getCanvas();

		// Clear background
		canvas->clear(SK_ColorWHITE);

		SkPaint paint;
		paint.setColor(SK_ColorRED);

		// Draw a rectangle with red paint
		SkRect rect = SkRect::MakeXYWH(10, 10, 128, 128);
		canvas->drawRect(rect, paint);

		// Set up a linear gradient and draw a circle
		{
			SkPoint linearPoints[] = { { 0, 0 }, { 300, 300 } };
			SkColor linearColors[] = { SK_ColorGREEN, SK_ColorBLACK };
			paint.setShader(SkGradientShader::MakeLinear(linearPoints, linearColors, nullptr, 2,
				SkTileMode::kMirror));
			paint.setAntiAlias(true);

			canvas->drawCircle(200, 200, 64, paint);

			// Detach shader
			paint.setShader(nullptr);
		}

		// Draw a message with a nice black paint
		SkFont font;
		font.setSubpixel(true);
		font.setSize(20);
		paint.setColor(SK_ColorBLACK);

		canvas->save();
		static const char message[] = "Hello World";

		// Translate and rotate
		canvas->translate(300, 300);
		float fRotationAngle = 0.2f;
		if (fRotationAngle > 360) {
			fRotationAngle -= 360;
		}
		canvas->rotate(fRotationAngle);

		// Draw the text
		canvas->drawSimpleText(message, strlen(message), SkTextEncoding::kUTF8, 0, 0, font, paint);

		float seconds = fmod((float) this->time.get_seconds(), (float) animation->duration());
		animation->seek(seconds / (float)animation->duration());
		auto rect2 = SkRect::MakeXYWH(10, 10, 500, 500);
		animation->render(canvas, &rect2);

		canvas->restore();

		canvas->readPixels(info, data.data(), 4 * get_width(), 0, 0);
	}

private:
	sk_sp<SkSurface> surface = nullptr;
	std::vector<uint8_t> data;
	SkImageInfo info;

	std::string json_data;

	sk_sp<skottie::Animation> animation;

};

int main() {
	SkiaApp skia;
	skia.set_dimensions(1024, 1024);
	skia.init();
	skia.run();
	return 0;
}
