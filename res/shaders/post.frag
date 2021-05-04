#version 410 core

#define MAX_LIGHTS 32
#define SMOOTHNESS 0.5
#define DITHERING_THRESHOLD 0.85

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

//Palette
uniform sampler2D palette;
uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;

//Light
uniform vec4 light[MAX_LIGHTS];
uniform int light_size;
uniform float light_distortion;
uniform bool use_light;

//Settings
uniform bool is_background;
uniform bool use_grayscale;

float smooth_min(float a, float b, float k) {
    float h = max(k-abs(a-b), 0) / k;
    return min(a,b) - h*h*h*k*1/6.0;
}

void main() {
    //Sample color from texture
    color = texture(tex, f_tex_coord);
    
    //Luminance
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    //Light
    if (use_light) {
        float light_accumulator = 1.0;
        float light_dithering = 0.0;
        
        for (int i = 0; i < light_size; i++) {
            if (light[i].w <= 0)
                break;
            vec2 to_light = abs(light[i].xy - f_tex_coord);
            to_light.x *= light_distortion;
            float dst_to_light = sqrt(to_light.x * to_light.x + to_light.y * to_light.y);
            
            float light_at_point = (dst_to_light - light[i].w) / (light[i].z - light[i].w);
            light_accumulator = (dst_to_light > light[i].w) ? smooth_min(light_at_point, light_accumulator, SMOOTHNESS) : 0.0;
            if (light_accumulator < 0.0)
                light_accumulator = 0.0;
        }
        
        float light_at_point = 1.0 - light_accumulator;
        
        luminance *= light_at_point;
        
        if (luminance < 0.0)
            luminance = 0.0;
    }
    
    //Palette
    if (palette_index > -1.0f) {
        vec4 i_color;
    
        if (use_grayscale) {
            i_color = luminance * vec4(1.0, 1.0, 1.0, 1.0);
        } else {
            i_color = texture(palette, vec2(luminance * 0.999, palette_index));
            if (previous_palette_index != palette_index) {
                vec4 p_color = texture(palette, vec2(luminance * 0.999, previous_palette_index));
                i_color = transition_percent * i_color + (1.0-transition_percent) * p_color;
            }
        }
        
        color = vec4(i_color.rgb, color.a);
    }
}
