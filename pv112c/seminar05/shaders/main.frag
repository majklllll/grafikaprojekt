#version 330

out vec4 final_color;

in vec3 VS_normal_ws;
in vec3 VS_position_ws;
in vec2 VS_tex_coord;

uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

uniform vec4 light_position;
uniform vec3 light_ambient_color;
uniform vec3 light_diffuse_color;
uniform vec3 light_specular_color;

uniform vec3 eye_position;

void main()
{
    const vec3 red = vec3(1.0, 0.3, 0.0);
    const vec3 white = vec3(1.0, 1.0, 1.0);

    // TASK 1: Create red-white stripes. Start with a simple gradient, then
    //   repeat it. Try a different mixing function for better results
    // TASK 2: Combine vertical and horizontal stripes of different sizes to
    //   create a grid resembling a brick wall.
    // TASK 3: Offset every other row a bit to create a better brick wall.

    vec3 tex_color = vec3(1.0);

    // TASK 4a: Comment out the bricks. Create a red circle in the center of
    //   the cube (so that it looks a bit like the flag of Japan).
    // TASK 4a: Repeat the circles to create a polka-dot pattern. Then draw a
    //   teapot instead of the cube.

    // vec3 tex_color = vec3(1.0);

    vec3 N = normalize(VS_normal_ws);
    vec3 Eye = normalize(eye_position - VS_position_ws);

    vec3 L = normalize(light_position.xyz - VS_position_ws * light_position.w);

    vec3 H = normalize(L + Eye);

    float Idiff = max(dot(N, L), 0.0);
    float Ispec = Idiff * pow(max(dot(N, H), 0.0), material_shininess);

    vec3 mat_ambient = material_ambient_color * tex_color;
    vec3 mat_diffuse = material_diffuse_color * tex_color;
    vec3 mat_specular = material_specular_color;

    vec3 light =
        mat_ambient * light_ambient_color +
        mat_diffuse * light_diffuse_color * Idiff +
        mat_specular * light_specular_color * Ispec;

    final_color = vec4(light, 1.0);
}
