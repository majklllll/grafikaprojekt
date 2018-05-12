#version 330

in vec3 position;

// TASK 1: uncomment projection matrix
uniform mat4 projection_matrix;
// TASK 2: uncomment view matrix
uniform mat4 view_matrix;
// TASK 3: uncomment model matrix
uniform mat4 model_matrix;

void main()
{
    // TASK 1: multiply position by projection matrix
    // TASK 2: multiply position by view matrix
    // TASK 3: multiply position by model matrix
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
