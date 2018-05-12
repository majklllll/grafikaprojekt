#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <iostream>

using namespace std;

void Application::init() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  program = make_unique<ShaderProgram>("shaders/main.vert", "shaders/main.frag");

  // Get locations of vertex attributes position and normal
  GLint position_loc = program->get_attribute_location("position");
  GLint normal_loc = program->get_attribute_location("normal");

  // Get locations of uniforms for positioning and projecting object
  model_matrix_loc = program->get_uniform_location("model_matrix");
  view_matrix_loc = program->get_uniform_location("view_matrix");
  projection_matrix_loc = program->get_uniform_location("projection_matrix");

  // Get location of color uniform
  //time_loc = program->get_uniform_location("time");

  light_position_loc = program->get_uniform_location("light_position");
  eye_position_loc = program->get_uniform_location("eye_position");
  light_ambient_color_loc = program->get_uniform_location("light_ambient_color");
  light_diffuse_color_loc = program->get_uniform_location("light_diffuse_color");
  light_specular_color_loc = program->get_uniform_location("light_specular_color");

  material_ambient_color_loc = program->get_uniform_location("material_ambient_color");
  material_diffuse_color_loc = program->get_uniform_location("material_diffuse_color");
  material_specular_color_loc = program->get_uniform_location("material_specular_color");
  material_shininess_loc = program->get_uniform_location("material_shininess");

  teapot.create_vao(position_loc, normal_loc);

  meshes = Mesh::from_file("objects/projekt.obj");
  materials = Mesh::loadMaterials("objects/projekt.obj");


  super_cube = (*meshes.at(0));
  //super_whatever = (*meshes.at(1));

  //(super_cube).create_vao(position_loc, normal_loc);
  //(super_whatever).create_vao(position_loc, normal_loc);

  for(auto const& mesh: meshes) {
      (*mesh).create_vao(position_loc, normal_loc);
  }
}

void Application::render() {

    // Get the current time
    float time = float(glfwGetTime());

    // Get the aspect ratio of window size
    float width = float(window.get_width());
    float height = float(window.get_height());
    float aspect_ratio = width / height;

    // Clear screen, both color and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind(use) our program
    program->use();


    glm::mat4 projection_matrix = glm::perspective(
                glm::radians(45.0f),
                aspect_ratio,
                0.1f,
                100.0f);

    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glm::mat4 view_matrix = glm::lookAt(camera.get_eye_position(), camera.get_center_of_view(), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform3fv(eye_position_loc, 1, glm::value_ptr(camera.get_eye_position()));
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    glUniform4f(light_position_loc, 2.0f, 25.0f, 2.0f, 0.0f);
    glUniform3f(light_ambient_color_loc, 0.02f, 0.02f, 0.02f);
    glUniform3f(light_diffuse_color_loc, 1.0f, 1.0f, 1.0f);
    glUniform3f(light_specular_color_loc, 1.0f, 1.0f, 1.0f);

    //cube.bind_vao();

    for(size_t i=0; i < meshes.size(); i++) {
        drawMesh(*meshes[i], i);
        (*meshes[i]).draw();
    }

//5      glm::mat4 cube_model_matrix = glm::mat4(1.0);
//5      glm::mat4 model_matrix = cube_model_matrix; //glm::rotate(cube_model_matrix, time, glm::vec3(0.0f, 1.0f, 0.0f));
//5      glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

      //glUniform1f(time_loc, time);
}



void Application::drawMesh(Mesh mesh, size_t material_index = -1) {
    mesh.bind_vao();
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    // The materials are set for every object
    /*glUniform3f(material_ambient_color_loc, 0.329412f, 0.223529f, 0.027451f);
    glUniform3f(material_diffuse_color_loc, 0.780392f, 0.568627f, 0.113725f);
    glUniform3f(material_specular_color_loc, 0.992157f, 0.941176f, 0.807843f);
    glUniform1f(material_shininess_loc, 27.8974f);*/

    if(material_index != -1)
        set_material(material_index);
}

void Application::set_material(int index)
{
    int R=0,G=1,B=2;
    if(index >= 0) {
        glUniform3f(material_ambient_color_loc, materials[index].ambient[R], materials[index].ambient[G], materials[index].ambient[B]);
        glUniform3f(material_diffuse_color_loc, materials[index].diffuse[R], materials[index].diffuse[G], materials[index].diffuse[B]);
        glUniform3f(material_specular_color_loc, materials[index].specular[R], materials[index].specular[G], materials[index].specular[B]);
        glUniform1f(material_shininess_loc, 27.8974f);
    }
}


void Application::on_mouse_position(double x, double y) {
    camera.on_mouse_move(x, y);
}

void Application::on_mouse_button(int button, int action, int mods) {
    camera.on_mouse_button(button, action, mods);
}

void Application::on_key(int key, int scancode, int actions, int mods) {
    switch (key) {
    case GLFW_KEY_L:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case GLFW_KEY_F:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLFW_KEY_W:
        camera.cam.x--;
        camera.update_eye_pos();
        break;
    case GLFW_KEY_S:
        camera.cam.x++;
        camera.update_eye_pos();
        break;
    case GLFW_KEY_A:
        camera.cam.z++;
        camera.update_eye_pos();
        break;
    case GLFW_KEY_D:
        camera.cam.z--;
        camera.update_eye_pos();
        break;
    }
}

void Application::on_resize(int width, int height) {
    window.resize(width, height);

    // Set the area into which we render
    glViewport(0, 0, width, height);
}
