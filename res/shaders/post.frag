#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D tex;

//Palette
uniform sampler2D palette;
uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;

//Light
uniform vec4 light[];
uniform float light_distortion;

//Settings
uniform bool is_background;
uniform bool use_grayscale;

void main() {
    color = texture(tex, f_tex_coord);
    
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    vec2 to_light = abs(light[0].xy - f_tex_coord);
    to_light.x *= light_distortion;
    float dst_to_light = sqrt(to_light.x * to_light.x + to_light.y * to_light.y);
    
    if (dst_to_light > light[0].w)
        luminance *= 1.0 - ((dst_to_light - light[0].w) / (light[0].z - light[0].w));
    
    if (luminance < 0.001)
        luminance = 0.001;
    
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
