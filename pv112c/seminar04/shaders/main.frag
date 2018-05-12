#version 330

uniform sampler2D texture_primary;
uniform sampler2D texture_secondary;

in vec3 vertex_position_ws;
in vec3 vertex_normal_ws;
in vec2 vertex_texture_coordinate;

out vec4 final_color;

void main() {
    // Task 7: Use texture color sampled by function texture(sampler, coordinats)
    // Task 8: Where color of texture_secondary == vec3(1.0, 1.0, 1.0) => set color to be one of texture_primary

    // Task 12: comment code above and use only primary texture now INCLUDING ALPHA
    final_color = texture(texture_primary, vertex_texture_coordinate).rgba;

}
