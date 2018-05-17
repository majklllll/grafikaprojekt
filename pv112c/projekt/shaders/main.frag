#version 330

//uniform vec3 color;
//uniform float time;

uniform samplerCube skybox;


uniform vec3 eye_position;
in vec3 vertex_position_ws;
in vec3 vertex_normal_ws;

uniform vec4 light_position;
uniform vec3 light_ambient_color;
uniform vec3 light_diffuse_color;
uniform vec3 light_specular_color;
uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

in vec3 TexCoords;


out vec4 final_color;

void main()
{
  /*vec3 L = normalize(light_position.xyz - vertex_position_ws * light_position.w);
  vec3 N = normalize(vertex_normal_ws);
  vec3 E = normalize(eye_position - vertex_position_ws);
  vec3 H = normalize(E + L);

  // Distance of the current fragment from the light source; 0 if directional
  float d = distance(light_position.xyz, vertex_position_ws) * light_position.w;

  // Diffuse light intensity
  float diffuse_intensity = max(dot(L, N), 0.0);
  if (d > 0.0) {
      // Attenuation factor
      // Here we use inverse-square law (https://en.wikipedia.org/wiki/Inverse-square_law)
      float attenuation = 1.0 / (d * d);
      diffuse_intensity *= attenuation;
  }

  // Specular light intensity
  float specular_intensity = pow(max(dot(N, H), 0.0), material_shininess);

  // Final light
  vec3 light = light_ambient_color * material_ambient_color +
               diffuse_intensity * light_diffuse_color * material_diffuse_color +
               specular_intensity * light_specular_color * material_specular_color;

  final_color = vec4(light, 1.0);*/





    final_color = vec4(vec3(0.0,1.0,0.0), 1.0);
}
