#version 330

//uniform vec3 color;
//uniform float time;

struct light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float square;
};

uniform light pointLights[5];




uniform sampler2D texture_primary;
uniform bool use_texture;

uniform vec3 eye_position;

in vec3 vertex_position_ws;
in vec3 vertex_normal_ws;
in vec2 vertex_texture_coordinate;

uniform vec4 light_position;
uniform vec3 light_ambient_color;
uniform vec3 light_diffuse_color;
uniform vec3 light_specular_color;

uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

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
  float attenuation = 1.0 / (pow(d, 2) * 0.032f + d * 0.09f + 1.0f);

  // Specular light intensity
  float specular_intensity = pow(max(dot(N, H), 0.0f), material_shininess);*/

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 light_pos = light_position.xyz;



  // diffuse
  vec3 norm = normalize(vertex_normal_ws);
  vec3 lightDir = normalize(light_pos - vertex_position_ws);
  float diff = max(dot(norm, lightDir), 0.0);


  // specular
  vec3 viewDir = normalize(eye_position - vertex_position_ws);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);

  if(use_texture) {
    ambient = light_ambient_color * texture(texture_primary, vertex_texture_coordinate).rgb;
    diffuse = light_diffuse_color * diff * texture(texture_primary, vertex_texture_coordinate).rgb;
    specular = light_specular_color * spec * texture(texture_primary, vertex_texture_coordinate).rgb;
  } else {
    ambient = light_ambient_color * material_ambient_color;
    diffuse = light_diffuse_color * diff * material_diffuse_color;
    specular = light_specular_color * spec * material_specular_color;


  }





  // attenuation
  float distance    = length(light_pos - vertex_position_ws);
  float attenuation = 1.0 / (pow(distance, 2) * 0.032f + distance * 0.09f + 1.0f);

  ambient  *= attenuation;
  diffuse   *= attenuation;
  specular *= attenuation;

  vec3 result = ambient + diffuse + specular;
  final_color = vec4(result, 1.0);


  /*


  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 resulted;


  if(use_texture) {
    float tex_alpha = texture(texture_primary, vertex_texture_coordinate).a;
    vec3 tex_color = texture(texture_primary, vertex_texture_coordinate).rgb;

    // Final light
    ambient = light_ambient_color * tex_color;
    diffuse = diffuse_intensity * tex_color * material_diffuse_color;
    specular = specular_intensity * light_specular_color * material_specular_color;
  } else {

      // Final light
      ambient = light_ambient_color * material_ambient_color;
      diffuse = diffuse_intensity * light_diffuse_color * material_diffuse_color;
      specular = specular_intensity * light_specular_color * material_specular_color;
  }
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  resulted = ambient + diffuse + specular;
  final_color = vec4(resulted, 1.0f);

*/



}
