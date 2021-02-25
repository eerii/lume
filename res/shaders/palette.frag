varying vec2 v_texCoord;

uniform sampler2D tex;
uniform sampler2D palette;
uniform float palette_index;

void main() {
    vec4 color = texture2D(tex, v_texCoord);
    float luminance = (color.x + color.y + color.z) / 3.0;
    vec4 i_color = texture2D(palette, vec2(luminance, palette_index));
    gl_FragColor = vec4(i_color.rgb, color.a);
}
