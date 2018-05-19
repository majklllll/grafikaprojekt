#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <iostream>

using namespace std;

void Application::initialize_locs()
{
    // Get locations of uniforms for positioning and projecting object
    model_matrix_loc = program->get_uniform_location("model_matrix");
    view_matrix_loc = program->get_uniform_location("view_matrix");
    projection_matrix_loc = program->get_uniform_location("projection_matrix");

    view_matrix_skybox_loc = skybox->get_uniform_location("view_matrix");
    projection_matrix_skybox_loc = skybox->get_uniform_location("projection_matrix");

    // Get location of color uniform
    //time_loc = program->get_uniform_location("time");


    texture_loc = program->get_uniform_location("texture_primary");
    use_texture_loc = program->get_uniform_location("use_texture");

    light_position_loc = program->get_uniform_location("light_position");
    eye_position_loc = program->get_uniform_location("eye_position");
    light_ambient_color_loc = program->get_uniform_location("light_ambient_color");
    light_diffuse_color_loc = program->get_uniform_location("light_diffuse_color");
    light_specular_color_loc = program->get_uniform_location("light_specular_color");

    material_ambient_color_loc = program->get_uniform_location("material_ambient_color");
    material_diffuse_color_loc = program->get_uniform_location("material_diffuse_color");
    material_specular_color_loc = program->get_uniform_location("material_specular_color");
    material_shininess_loc = program->get_uniform_location("material_shininess");
}

void Application::loadObjFiles()
{
    meshes = Mesh::from_file("objects/house.obj");
    std::vector<tinyobj::material_t>  mats = Mesh::loadMaterials("objects/house.obj");

    for(auto &mate :mats) {
        material new_mat;
        new_mat.mat = mate;
        new_mat.texture_id = -1;

        if(mate.diffuse_texname != "") {
            new_mat.texture_id = load_texture_2d(mate.diffuse_texname);
            glBindTexture(GL_TEXTURE_2D, new_mat.texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(border_color));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        materials[mate.name] = new_mat;
    }
}

void Application::create_vaos(GLint normal_loc, GLint position_loc, GLint textur_coord_loc)
{
    for(auto const& mesh: meshes) {
        (*mesh).create_vao(position_loc, normal_loc, textur_coord_loc);
    }
}

void Application::init() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  program = make_unique<ShaderProgram>("shaders/main.vert", "shaders/main.frag");
  skybox = make_unique<ShaderProgram>("shaders/skybox.vert", "shaders/skybox.frag");

  // Get locations of vertex attributes position and normal
  GLint position_loc = program->get_attribute_location("position");
  GLint normal_loc = program->get_attribute_location("normal");
  GLint textur_coord_loc = program->get_attribute_location("texture_coordinate");

  GLint position_skybox_loc = skybox->get_attribute_location("position");

  initialize_locs();
  loadObjFiles();

  cube.create_vao(position_skybox_loc, -1);

  create_vaos(normal_loc, position_loc, textur_coord_loc);
  createSkyBox();
}

void Application::set_vertex_matrices()
{
    // Get the aspect ratio of window size
    float width = float(window.get_width());
    float height = float(window.get_height());
    float aspect_ratio = width / height;

    glm::mat4 projection_matrix = glm::perspective(
                glm::radians(45.0f),
                aspect_ratio,
                0.1f,
                10000.0f);

    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glm::mat4 view_matrix = glm::lookAt(camera.get_eye_position(), camera.get_center_of_view(), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform3fv(eye_position_loc, 1, glm::value_ptr(camera.get_eye_position()));
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
}

void Application::render() {

    // Get the current time
    float time = float(glfwGetTime());

    // Clear screen, both color and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render blue sky
    render_sky_box();

    // Bind normal shader program
    program->use();
    set_vertex_matrices();

    glUniform4f(light_position_loc, 250.0f, 250.0f, 250.0f, 0.0f);
    glUniform3f(light_ambient_color_loc, 0.02f, 0.02f, 0.02f);
    glUniform3f(light_diffuse_color_loc, 1.0f, 1.0f, 1.0f);
    glUniform3f(light_specular_color_loc, 1.0f, 1.0f, 1.0f);


    /*for(size_t i=0; i < meshes.size(); i++) {
        drawMesh(*meshes[i], 0);
        (*meshes[i]).draw();
    }*/
    drawMesh(*meshes[0], materials["kdosi"]);
    (*meshes[0]).draw();

    drawMesh(*meshes[1], materials["kdosi"]);
    (*meshes[1]).draw();

    drawMesh(*meshes[2], materials["kdosi"]);
    (*meshes[2]).draw();

    drawMesh(*meshes[3], materials["trava"]);
    (*meshes[3]).draw();

    drawMesh(*meshes[4], materials["cosi"]);
    (*meshes[4]).draw();

    drawMesh(*meshes[5], materials["mona"]);
    (*meshes[5]).draw();
      //glUniform1f(time_loc, time);
}



void Application::drawMesh(Mesh mesh, material& mater) {
    mesh.bind_vao();
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));


    set_material(mater);
}

void Application::createSkyBox()
{
    std::string faces[6] =
    {
        "right.png",
        "left.png",
        "up.png",
        "down.png",
        "front.png",
        "back.png",
    };

    skyboxID = load_texture_cubemap(faces);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Application::render_sky_box()
{
    // Get the aspect ratio of window size
    float width = float(window.get_width());
    float height = float(window.get_height());
    float aspect_ratio = width / height;

    glDepthMask(GL_FALSE);
    skybox->use();


    glm::mat4 projection_matrix = glm::perspective(
                glm::radians(45.0f),
                aspect_ratio,
                0.1f,
                1000000.0f);

    glUniformMatrix4fv(projection_matrix_skybox_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glm::mat4 view_matrix = glm::lookAt(camera.get_eye_position(), camera.get_center_of_view(), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(view_matrix_skybox_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    cube.bind_vao();

    //glUniform1i(skybox_loc, 0);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
    cube.draw();

    glDepthMask(GL_TRUE);
}

void Application::set_material(material &mater)
{
    cout << mater.mat.name  << mater.texture_id << "  " << UNDEFINED << endl;
    auto &mat = mater.mat;
    int R=0,G=1,B=2;

    glUniform3f(material_ambient_color_loc, mat.ambient[R], mat.ambient[G], mat.ambient[B]);
    glUniform3f(material_diffuse_color_loc, mat.diffuse[R], mat.diffuse[G], mat.diffuse[B]);
    glUniform3f(material_specular_color_loc, mat.specular[R], mat.specular[G], mat.specular[B]);
    glUniform1f(material_shininess_loc, 1.0f);
    if(mater.texture_id != UNDEFINED) {
        glUniform1i(use_texture_loc, 1);
        glUniform1i(texture_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mater.texture_id);
    } else {
        glUniform1i(use_texture_loc, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
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
        camera.move_forward();
        break;
    case GLFW_KEY_S:
        camera.move_back();
        break;
    case GLFW_KEY_A:
        camera.turn_left();
        break;
    case GLFW_KEY_D:
        camera.turn_right();
        break;
    }
}

void Application::on_resize(int width, int height) {
    window.resize(width, height);

    // Set the area into which we render
    glViewport(0, 0, width, height);
}
