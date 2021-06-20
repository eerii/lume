#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;
uniform vec3 text_color;
uniform bool same_color;

void main() {
    float c = texture(tex, f_tex_coord).r;
    
    float alpha = 1.0f;
    if (c < 0.1f)
        alpha = 0.0f;
    
    if (same_color)
        color = vec4(text_color, alpha);
    else
        color = vec4(text_color * c, alpha);
    
    if (color.a == 0.0f)
        discard;
}
