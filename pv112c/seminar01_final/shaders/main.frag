#version 330

// The only output variable of fragment shader is the fragment's color
out vec4 final_color;

// We get this color from vertex shader
in vec3 color_VS;

void main()
{
    final_color = vec4(color_VS, 1.0);
}
