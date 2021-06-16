#version 410 core

in vec2 f_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D flame;
uniform sampler2D gradient;
uniform sampler2D noise;

#define GRADIENT_CONTRIBUTION 0.3f
#define NOISE_STRENGTH 0.85f

void main() {
    float flame_value = texture(flame, f_tex_coord).x;
    float noise_value = texture(noise, f_tex_coord).x;
    vec2 f_tex_coord_t = vec2(1.0f - f_tex_coord.y, f_tex_coord.x);
    float grad_value = texture(gradient, f_tex_coord_t).x;
    
    color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (flame_value > 0.0f) {
        float combined = flame_value - GRADIENT_CONTRIBUTION * grad_value;
        float s1 = step(noise_value * NOISE_STRENGTH, combined);
        
        if (s1 > 0.1f) {
            color = vec4(s1, s1, s1, 1.0f);
        }
    }
}

