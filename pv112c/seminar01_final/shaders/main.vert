#version 330

// From buffers, we get position and color for each vertex
in vec2 position;
in vec3 color;

// To fragment shader, we send the color
// GL interpolates the color between vertices
out vec3 color_VS;

// From C++ code, we get the window's aspect ratio
uniform float aspect;

void main()
{
    // Pass the color to fragment shader
    color_VS = color;

    // Aspect ratio adjustment
    vec2 final_position = position;
    final_position.x /= aspect;

    // gl_Position is a built-in variable, where we store the vertex' position
    gl_Position = vec4(final_position, 0.0, 1.0);
}
