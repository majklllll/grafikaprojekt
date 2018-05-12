#version 330

out vec4 final_color;
in vec3 colorVS;

void main()
{
    final_color = vec4(colorVS, 1.0f);
}
