#version 330

in vec2 position;

out vec2 vertex_texture_coordinate;

void main() {
    vertex_texture_coordinate = position * 0.5 + 0.5;
    gl_Position = vec4(position, 0.0, 1.0);
}
