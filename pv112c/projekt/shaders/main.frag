#version 330

//uniform vec3 color;
//uniform float time;
#define MAX_LIGHTS 100
 struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float square;
};

uniform Light lights[1];
uniform int light_count;




uniform sampler2D texture_primary;
uniform bool use_texture;

uniform vec3 eye_position;

in vec3 vertex_position_ws;
in vec3 vertex_normal_ws;
in vec2 vertex_texture_coordinate;

uniform vec3 material_ambient_color;
uniform vec3 material_diffuse_color;
uniform vec3 material_specular_color;
uniform float material_shininess;

out vec4 final_color;

void main()
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // diffuse
  vec3 norm = normalize(vertex_normal_ws);
  vec3 lightDir = normalize(lights[0].position - vertex_position_ws);
  float diff = max(dot(norm, lightDir), 0.0);

  // specular
  vec3 viewDir = normalize(eye_position - vertex_position_ws);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);

  if(use_texture) {
    ambient = lights[0].ambient * texture(texture_primary, vertex_texture_coordinate).rgb;
    diffuse = lights[0].diffuse * diff * texture(texture_primary, vertex_texture_coordinate).rgb;
    specular = lights[0].specular * spec * texture(texture_primary, vertex_texture_coordinate).rgb;
  } else {
    ambient = lights[0].ambient * material_ambient_color;
    diffuse = lights[0].diffuse * diff * material_diffuse_color;
    specular = lights[0].specular * spec * material_specular_color;
  }

  // attenuation
  float distance    = length(lights[0].position - vertex_position_ws);
  float attenuation = 1.0 / (pow(distance, 2) * 0.032f + distance * 0.09f + 1.0f);

  ambient  *= attenuation;
  diffuse   *= attenuation;
  specular *= attenuation;

  vec3 result = ambient + diffuse + specular;
  final_color = vec4(result, 1.0);


}
