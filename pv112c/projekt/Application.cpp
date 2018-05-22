#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


using namespace std;

void Application::initialize_locs()
{
    // Get locations of uniforms for positioning and projecting object
    model_matrix_loc = program->get_uniform_location("model_matrix");
    view_matrix_loc = program->get_uniform_location("view_matrix");
    projection_matrix_loc = program->get_uniform_location("projection_matrix");

    view_matrix_skybox_loc = skybox->get_uniform_location("view_matrix");
    projection_matrix_skybox_loc = skybox->get_uniform_location("projection_matrix");

    texture_loc = program->get_uniform_location("texture_primary");
    use_texture_loc = program->get_uniform_location("use_texture");

    bump_loc = program->get_uniform_location("texture_bump");
    use_bump_loc = program->get_uniform_location("use_bump");

    light_count_loc = program->get_uniform_location("light_count");
    eye_position_loc = program->get_uniform_location("eye_position");

    material_ambient_color_loc = program->get_uniform_location("material_ambient_color");
    material_diffuse_color_loc = program->get_uniform_location("material_diffuse_color");
    material_specular_color_loc = program->get_uniform_location("material_specular_color");
    material_shininess_loc = program->get_uniform_location("material_shininess");
    material_alpha_loc = program->get_uniform_location("material_alpha");
}

void Application::loadObjFiles()
{
    meshes = Mesh::from_file("objects/house.obj");
    std::vector<tinyobj::material_t> mats = Mesh::loadMaterials("objects/house.obj");

    for(auto &mate :mats) {
        material new_mat;
        new_mat.mat = mate;
        new_mat.texture_id = -1;
        new_mat.texture_bump_id = -1;

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
        if(mate.bump_texname != "") {
            new_mat.texture_bump_id = load_texture_2d(mate.bump_texname);
            glBindTexture(GL_TEXTURE_2D, new_mat.texture_bump_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(border_color));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            cout << mate.bump_texname << endl;
        }
        new_mat.transparency = mate.dissolve;
        new_mat.refr_index = mate.ior;

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
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //glEnable(GL_DITHER);
  glCullFace(GL_FRONT);




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

  set_lights();

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

std::vector<Application::light> Application::get_lights()
{

    std::vector<light> lights;

    // Lights on ceiling
    for(int i=0; i<4; i++) {
        float z = 7.0f - 4.0f * i;
        for(int j=0;j<2;j++) {
            float x = -2.0f - 6.0f * j;
            light l0;
            l0.position = glm::vec3(x, 3.5f, z);
            l0.ambient = glm::vec3( 0.05f, 0.05f, 0.05f);
            l0.diffuse = glm::vec3( 0.8f, 0.8f, 0.9f);
            l0.specular = glm::vec3( 1.0f, 1.0f, 1.0f);
            l0.square = 0.045f;
            l0.linear = 0.15f;
            l0.constant = 1.0f;
            lights.push_back(l0);
        }
    }

    light moon;
    moon.position = glm::vec3(500.0f, 500.0f, 500.0f);
    moon.ambient = glm::vec3( 0.05f, 0.05f, 0.05f);
    moon.diffuse = glm::vec3( 0.99f, 0.99f, 0.99f);
    moon.specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    moon.square = 0.0000032f;
    moon.linear = 0.00009f;
    moon.constant = 1.0f;
    lights.push_back(moon);

    return lights;
}

void Application::set_lights()
{
    program->use();
    GLuint shader = program->program_id;
    vector<light> lights = get_lights();
    for(size_t i = 0; i < lights.size(); i++) {
        string lstr = string("lights[") + to_string(i) + "].";
        cout << lstr << endl;
        set_vec3(shader, lstr + "position", lights[i].position);
        set_vec3(shader, lstr + "ambient", lights[i].ambient);
        set_vec3(shader, lstr + "diffuse", lights[i].diffuse);
        set_vec3(shader, lstr + "specular", lights[i].specular);

        set_float(shader, lstr + "square", lights[i].square);
        set_float(shader, lstr + "linear", lights[i].linear);
        set_float(shader, lstr + "constant", lights[i].constant);
    }
    set_int(shader, "light_count", (int)lights.size());
    cout << "Svetel:" << to_string(lights.size()) << endl;
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

    // House
    drawMesh(*meshes[0], materials["kdosi"]);
    (*meshes[0]).draw();
    drawMesh(*meshes[1], materials["kdosi"]);
    (*meshes[1]).draw();

    //Mona lisa
    drawMesh(*meshes[2], materials["mona"]);
    (*meshes[2]).draw();

    // Point LED lights on ceiling
    for(int i=0;i<16;i+=2) {
        drawMesh(*meshes[3+i], materials["sklo"]);
        (*meshes[3+i]).draw();
        drawMesh(*meshes[4+i], materials["white_plastic"]);
        (*meshes[4+i]).draw();
    }

    // Window
    drawMesh(*meshes[20], materials["ram_okna"]);
    (*meshes[20]).draw();



    // Door

    drawMesh(*meshes[22], materials["ram_okna"]);
    (*meshes[22]).draw();

    drawMesh(*meshes[24], materials["gold"]);
    (*meshes[24]).draw();

    // Floor
    drawMesh(*meshes[25], materials["floor"]);
    (*meshes[25]).draw();

    // Carpet
    drawMesh(*meshes[26], materials["carpet"]);
    (*meshes[26]).draw();

    // Fireplace
    drawMesh(*meshes[27], materials["fireplace"]);
    (*meshes[27]).draw();
    drawMesh(*meshes[28], materials["krbrimsa"]);
    (*meshes[28]).draw();
    drawMesh(*meshes[29], materials["stones"]);
    (*meshes[29]).draw();
    drawMesh(*meshes[30], materials["stones"]);
    (*meshes[30]).draw();

    // Painting Lady
    drawMesh(*meshes[31], materials["lady"]);
    (*meshes[31]).draw();

    // Cokoo clock
    drawMesh(*meshes[32], materials["black"]);
    (*meshes[32]).draw();
    drawMesh(*meshes[33], materials["black"]);
    (*meshes[33]).draw();
    drawPendulum(*meshes[34], materials["gold"]);
    (*meshes[34]).draw();
    drawMesh(*meshes[35], materials["ram_okna"]);
    (*meshes[35]).draw();
    drawMesh(*meshes[36], materials["floor"]);
    (*meshes[36]).draw();

    drawBlendedMeshes();
    //glUniform1f(time_loc, time);
}
void Application::drawBlendedMeshes()
{
    drawMesh(*meshes[21], materials["sklo"]);
    (*meshes[21]).draw();
    drawMesh(*meshes[19], materials["sklo"]);
    (*meshes[19]).draw();
    drawMesh(*meshes[23], materials["sklo"]);
    (*meshes[23]).draw();

}

void Application::drawMesh(Mesh mesh, material& mater) {
    mesh.bind_vao();
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
    set_material(mater);
}

void Application::drawPendulum(Mesh mesh, material& mater)
{
    float time = float(glfwGetTime());
    mesh.bind_vao();
    glm::mat4 model_matrix = glm::mat4(1.0f);


    model_matrix = glm::translate(model_matrix, glm::vec3(-5.059f, 1.7f, 9.65f));
    model_matrix = glm::rotate(model_matrix, glm::sin(4.0f*time)*0.5f, glm::vec3(0.0f, 0.0f, 1.0f));


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
                2000.0f);

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
    //cout << mater.mat.name  << mater.texture_id << "  " << UNDEFINED << endl;
    auto &mat = mater.mat;
    int R=0,G=1,B=2;


    glUniform3f(material_ambient_color_loc, mat.ambient[R], mat.ambient[G], mat.ambient[B]);
    glUniform3f(material_diffuse_color_loc, mat.diffuse[R], mat.diffuse[G], mat.diffuse[B]);
    glUniform3f(material_specular_color_loc, mat.specular[R], mat.specular[G], mat.specular[B]);
    glUniform1f(material_shininess_loc, 1.0f);
    glUniform1f(material_alpha_loc, mater.transparency);

    if(mater.texture_id != UNDEFINED) {
        glUniform1i(use_texture_loc, 1);
        glUniform1i(texture_loc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mater.texture_id);
    } else {
        glUniform1i(use_texture_loc, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glUniform1i(use_bump_loc, 0);
    if(mater.texture_bump_id != UNDEFINED) {
        glUniform1i(use_bump_loc, 1);
        glUniform1i(bump_loc, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mater.texture_bump_id);
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
