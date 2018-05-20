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

vec3 get_point_light(Light light, vec3 position_ws, vec3 normal, vec3 view_direction);

void main()
{
  vec3 normal = normalize(vertex_normal_ws);
  vec3 view_direction = normalize(eye_position - vertex_position_ws);
  vec3 result = vec3(0.0f,0.0f,0.0f);

  for(int i = 0; i < light_count; i++)
    result += get_point_light(lights[i], vertex_position_ws, normal, view_direction);

  final_color = vec4(result, 1.0);
}

vec3 get_point_light(Light light, vec3 position_ws, vec3 normal, vec3 view_direction) {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // diffuse
  vec3 light_direction = normalize(light.position - position_ws);
  float diff = max(dot(normal, light_direction), 0.0);

  // specular
  vec3 reflectDir = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflectDir), 0.0), material_shininess);

  if(use_texture) {
    ambient = light.ambient * texture(texture_primary, vertex_texture_coordinate).rgb;
    diffuse = light.diffuse * diff * texture(texture_primary, vertex_texture_coordinate).rgb;
    specular = light.specular * spec * texture(texture_primary, vertex_texture_coordinate).rgb;
  } else {
    ambient = light.ambient * material_ambient_color;
    diffuse = light.diffuse * diff * material_diffuse_color;
    specular = light.specular * spec * material_specular_color;
  }

  // attenuation
  float distance    = length(light.position - position_ws);
  float attenuation = 1.0 / (pow(distance, 2) * light.square + distance * light.linear + light.constant);

  ambient  *= attenuation;
  diffuse   *= attenuation;
  specular *= attenuation;

  vec3 result = ambient + diffuse + specular;
  return result;

}
