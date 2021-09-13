#version 410 core

#define MAX_LIGHTS 32
#define SMOOTHNESS 0.5

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;
uniform sampler2D light_tex;

//Light
uniform bool use_light;
uniform bool show_light;

//Palette
uniform sampler2D palette;
uniform float palette_index;
uniform float previous_palette_index;
uniform float palette_interval;
uniform float transition_percent;

//Settings
uniform bool is_background;
uniform bool use_grayscale;

float smooth_min(float a, float b, float k) {
    float h = max(k-abs(a-b), 0.0) / k;
    float v = float(h*h*h*k);
    return min(a,b) - v/6.0;
}

void main() {
    //Sample color from texture
    color = texture(tex, f_tex_coord);
    
    //Luminance
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    //Light
    if (use_light) {
        vec4 light_vec = texture(light_tex, f_tex_coord);
        float light = light_vec.x;
        luminance *= light;
    }
    
    if (luminance > 0.999)
        luminance = 0.999;
    
    //Palette
    if (palette_index > -1.0f) {
        vec4 i_color;
        
        if (use_grayscale) {
            i_color = luminance * vec4(1.0, 1.0, 1.0, 1.0);
        } else if (show_light) {
            i_color = texture(light_tex, f_tex_coord);
        } else {
            bool alt_palette = color.b < color.r;
            float adjusted_luminance = luminance * 0.5;
            
            if (alt_palette) {
                adjusted_luminance = luminance * (0.5 + palette_interval) + 0.5 - palette_interval;
                if (adjusted_luminance < 0.5)
                    adjusted_luminance = 0.0;
            }
            
            i_color = texture(palette, vec2(adjusted_luminance, palette_index));
            if (previous_palette_index != palette_index) {
                vec4 p_color = texture(palette, vec2(adjusted_luminance, previous_palette_index));
                i_color = transition_percent * i_color + (1.0-transition_percent) * p_color;
            }
        }
        
        color = vec4(i_color.rgb, color.a);
    }
}
