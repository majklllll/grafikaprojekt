#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec3 position;
in vec3 normal;
in vec2 texture_coordinate;

out vec3 vertex_position_ws;
out vec3 vertex_normal_ws;
out vec2 vertex_texture_coordinate;

void main()
{
  vertex_position_ws = vec3(model_matrix * vec4(position, 1.0));
  //vertex_normal_ws = normalize(inverse(transpose(mat3(model_matrix))) * normal);
  vertex_normal_ws = mat3(transpose(inverse(model_matrix))) * normal;
  vertex_texture_coordinate = texture_coordinate;

  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
