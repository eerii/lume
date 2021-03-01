varying vec2 v_tex_coord;

uniform sampler2D tex;
uniform sampler2D palette;
uniform sampler2D light;

uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;

uniform int is_background;

void main() {
    vec4 color = texture2D(tex, v_tex_coord);
    vec4 light_color = texture2D(light, v_tex_coord);
    
    color = vec4(color.rgb - light_color.rgb, color.a);
    
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    if (is_background == 1) {
        luminance = 0.0;
        color.a = 0.5;
    }
    
    vec4 i_color = texture2D(palette, vec2(luminance, palette_index));
    if (previous_palette_index != palette_index) {
        vec4 p_color = texture2D(palette, vec2(luminance, previous_palette_index));
        i_color = transition_percent * i_color + (1.0-transition_percent) * p_color;
    }
    
    gl_FragColor = vec4(i_color.rgb, color.a);
}
