#version 330

uniform samplerCube skybox;
in vec3 TexCoords;

out vec4 final_color;

void main()
{
    final_color = texture(skybox, TexCoords);
}
