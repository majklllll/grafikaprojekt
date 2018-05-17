#pragma once

#include <glad/glad.h>

#include <string>

#include "mesh.hpp"
#include "program.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "texture.hpp"

#include "tiny_obj_loader.h"

class Application {
public:
    Application(int initial_window_width, int initial_window_height, const std::string &title)
        : window(initial_window_width, initial_window_height, title) {
        // Set callbacks
        window.set_user_pointer(this);
        window.set_key_callback(on_key);
        window.set_size_callback(on_resize);
        window.set_mouse_button_callback(on_mouse_button);
        window.set_mouse_position_callback(on_mouse_position);
    }

  void init();
  void render();

  void on_key(int key, int scancode, int actions, int mods);
  void on_mouse_position(double x, double y);
  void on_mouse_button(int button, int action, int mods);
  void on_resize(int width, int height);

  // auxiliary methods
  void drawMesh(Mesh, size_t);
  void createSkyBox();
  void render_sky_box();



  // window must be first variable declared!
  Window window;

  void set_material(int material_index);

  void loadObjFiles();

  void create_vaos(GLint normal_loc, GLint position_loc);

  void set_vertex_matrices();

private:
  // application variables
  std::unique_ptr<ShaderProgram> program;
  std::unique_ptr<ShaderProgram> skybox;
  Camera camera;

  // Locations of uniforms for positioning and projecting object
  GLint model_matrix_loc = -1;
  GLint view_matrix_loc = -1;
  GLint projection_matrix_loc = -1;

  // Light related
  GLint light_position_loc = -1;
  GLint eye_position_loc = -1;

  // Light color
  GLint light_diffuse_color_loc = -1;
  GLint light_ambient_color_loc = -1;
  GLint light_specular_color_loc = -1;

  // Material color
  GLint material_ambient_color_loc = -1;
  GLint material_diffuse_color_loc = -1;
  GLint material_specular_color_loc = -1;
  GLint material_shininess_loc = -1;

  std::vector<std::unique_ptr<Mesh>> meshes;
  std::vector<tinyobj::material_t> materials;

  //skybox stuff
  GLint projection_matrix_skybox_loc = -1;
  GLint view_matrix_skybox_loc = -1;
  unsigned int skyboxID = -1;
  GLint skybox_loc = -1;

  Mesh cube = Mesh::skybox();


  static void on_key(GLFWwindow *window, int key, int scancode, int actions, int mods) {
      Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
      this_pointer->on_key(key, scancode, actions, mods);
  }

  static void on_mouse_position(GLFWwindow *window, double x, double y) {
      Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
      this_pointer->on_mouse_position(x, y);
  }

  static void on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
      Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
      this_pointer->on_mouse_button(button, action, mods);
  }

  static void on_resize(GLFWwindow *window, int width, int height) {
      Application *this_pointer = static_cast<Application *>(glfwGetWindowUserPointer(window));
      this_pointer->on_resize(width, height);
  }

  void initialize_locs();
};
