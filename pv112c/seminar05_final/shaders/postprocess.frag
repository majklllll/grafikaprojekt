#version 330

uniform float app_time_s;
uniform sampler2D scene_texture;

in vec2 vertex_texture_coordinate;

out vec4 final_color;

void main() {
    // Waves
    vec2 texture_coordinate_out = vertex_texture_coordinate;
    texture_coordinate_out.s += sin(4 * app_time_s + 40 * vertex_texture_coordinate.t) * 0.02;

    vec4 color = texture(scene_texture, texture_coordinate_out);

    // Weighted mean of colour channels
    vec3 color_weight = vec3(0.3, 0.6, 0.1);
    float grayscale = dot(color.rgb, color_weight);

    final_color = vec4(grayscale, grayscale, grayscale, 1.0);
}
