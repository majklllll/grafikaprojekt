#version 330

in vec2 position;

out vec2 VS_tex_coord;

void main()
{
    VS_tex_coord = position * 0.5 + 0.5;
    gl_Position = vec4(position, 0.0, 1.0);
}
