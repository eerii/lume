varying vec2 v_tex_coord;
varying vec2 n_light_source;
varying float n_light_radius;
varying float n_light_centre;
varying float light_distortion;

uniform sampler2D tex;
uniform sampler2D palette;

uniform float palette_index;
uniform float previous_palette_index;
uniform float transition_percent;

uniform int is_background;
uniform int use_grayscale;

void main() {
    vec4 color = texture2D(tex, v_tex_coord);
    
    float luminance = (color.x + color.y + color.z) / 3.0;
    
    vec2 to_light = abs(n_light_source - v_tex_coord);
    to_light.x *= light_distortion;
    float dst_to_light = sqrt(to_light.x * to_light.x + to_light.y * to_light.y);
    
    if (dst_to_light > n_light_centre)
        luminance *= 1.0 - ((dst_to_light - n_light_centre) / (n_light_radius - n_light_centre));
    
    if (luminance < 0.0)
        luminance = 0.0;
    
    if (is_background == 1) {
        luminance = 0.0;
        color.a = 0.5;
    }
    
    vec4 i_color;
    
    if (use_grayscale == 1) {
        i_color = luminance * vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        i_color = texture2D(palette, vec2(luminance, palette_index));
        if (previous_palette_index != palette_index) {
            vec4 p_color = texture2D(palette, vec2(luminance, previous_palette_index));
            i_color = transition_percent * i_color + (1.0-transition_percent) * p_color;
        }
    }
    
    gl_FragColor = vec4(i_color.rgb, color.a);
}
