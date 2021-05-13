#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, f_tex_coord);
    if (color.a == 0.0f)
        discard;
}
