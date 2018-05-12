#version 330

uniform vec3 eye_position;

uniform vec4 light_position;
uniform vec3 light_ambient_color;
uniform vec3 light_diffuse_color;
uniform vec3 light_specular_color;

uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

in vec3 vertex_position_ws;
in vec3 vertex_normal_ws;

out vec4 final_color;

void main() {
    // TASK 1:
    // Start with visualization of 'Lambert's cosine law'. Fragments color should be brighter
    // if the angle is smaller.
    // Hint 1: use dot() function for the angle
    // Hint 2: use max() or clamp() function to keep the value between 0.0 and 1.0
    // Hint 3: use .w component of light_position to differentiate point and directional light
    //         (relevant for (L)ight vector)

    // TASK 2:
    // Calculate the ambient part of the light

    // TASK 3:
    // Calculate the diffuse part of the light

    // TASK 4:
    // Calculate the specular part of the light
    // Hint 1: You have to add (H)alf vector

    // TASK 5:
    // Sum all the phong components into final color value

    // TASK 6:
    // Add ANY attenuation, for example Inverse Square Law
    // Hint: Do not apply any attenuation for Directional Light! Why?

    final_color = vec4(1.0, 0.0, 0.0, 1.0);
}
