#version 410 core

in vec2 f_tex_coord;
in vec2 f_noise_coord;

layout(location = 0) out vec4 color;

uniform sampler2D noise;
uniform sampler2D mask;

void main() {
    float noise_color = texture(noise, f_noise_coord).r;
    vec4 mask_color = texture(mask, f_tex_coord);
    
    if (mask_color.a == 0.0)
        discard;
    
    float s = step(noise_color, mask_color.r);
    if (s > 0.0)
        color = vec4(s, s, s, 1.0);
    else
        discard;
}


