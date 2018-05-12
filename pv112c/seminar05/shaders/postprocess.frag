#version 330

uniform float app_time_s;
uniform sampler2D sceneTex;

in vec2 VS_tex_coord;

out vec4 final_color;

void main()
{
    // TASK 6: Using post-processing, display the scene in grayscale.
    // TASK 7: Make the scene wave.

    vec2 tex_coord = VS_tex_coord;

    vec4 color = texture(sceneTex, tex_coord);

    final_color = color;
}
