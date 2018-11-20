
static const char *VertexShaderText = "\
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

static const char *FragmentShaderText = "\
varying vec2 Uv; \
uniform sampler2D Frame; \
void main() {\
  vec4 tex = texture2D(Frame, Uv); \n\
  gl_FragColor = tex.zyxw; \
}\
";
