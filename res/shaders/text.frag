#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

void main() {
    float c = texture(tex, f_tex_coord).r;
    
    float alpha = 1.0f;
    if (c < 0.1f)
        alpha = 0.0f;
    
    color = vec4(c, c, c, alpha);
    if (color.a == 0.0f)
        discard;
}
