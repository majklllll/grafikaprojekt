#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
in vec3 position;

out vec3 TexCoords;

void main()
{
  TexCoords = position;
  gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
}
