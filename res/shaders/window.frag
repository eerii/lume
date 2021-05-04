#version 410 core

#define MAX_LIGHTS 32
#define SMOOTHNESS 0.5
#define DITHERING_THRESHOLD 0.85

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

//Background
uniform sampler2D palette;
uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;
uniform bool use_grayscale;
uniform bool is_background;

void main() {
    if (is_background) {
        vec4 b_color = vec4(0.0f, 0.0f, 0.0f, 0.5f);
        
        if (palette_index > -1.0f && !use_grayscale) {
            b_color = texture(palette, vec2(0.0f, palette_index));
            if (previous_palette_index != palette_index) {
                vec4 p_color = texture(palette, vec2(0.0f, previous_palette_index));
                b_color = transition_percent * b_color + (1.0-transition_percent) * p_color;
            }
        }
        
        color = vec4(b_color.rgb, 0.5f);
    } else {
        color = texture(tex, f_tex_coord);
    }
}
