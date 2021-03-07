#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;
uniform sampler2D palette;

uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;

uniform bool is_background;
uniform bool use_grayscale;

void main() {
    color = texture(tex, f_tex_coord);
    
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    if (is_background) {
        luminance = 0.001;
        color.a = 0.5;
    }
    
    vec4 i_color;
    
    if (use_grayscale) {
        i_color = luminance * vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        i_color = texture(palette, vec2(luminance - 0.001, palette_index));
        if (previous_palette_index != palette_index) {
            vec4 p_color = texture(palette, vec2(luminance - 0.001, previous_palette_index));
            i_color = transition_percent * i_color + (1.0-transition_percent) * p_color;
        }
    }
    
    color = vec4(i_color.rgb, color.a);
}
