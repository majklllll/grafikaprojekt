#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.hpp"

using namespace std;

void Application::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Create shader program
    program = make_unique<ShaderProgram>("shaders/main.vert", "shaders/main.frag");

    // Get locations of vertex attributes position and normal
    GLint position_loc = program->get_attribute_location("position");
    GLint normal_loc = program->get_attribute_location("normal");
    GLint texture_coordinate_loc = program->get_attribute_location("texture_coordinate");

    // Get locations of uniforms for positioning and projecting object
    model_matrix_loc = program->get_uniform_location("model_matrix");
    view_matrix_loc = program->get_uniform_location("view_matrix");
    projection_matrix_loc = program->get_uniform_location("projection_matrix");

    cube.create_vao(position_loc, normal_loc, texture_coordinate_loc);
    sphere.create_vao(position_loc, normal_loc, texture_coordinate_loc);
    teapot.create_vao(position_loc, normal_loc, texture_coordinate_loc);

    // TASK 1: Study load_texture_2d function
    lenna_texture = load_texture_2d("images/Lenna.png");

    glBindTexture(GL_TEXTURE_2D, lenna_texture);
    // TASK 2: Set parameters of the texture
    //         2.1 WRAPPING - you can start with REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //         2.2 BORDER COLOR - check the documentation when is this parameter used
    glm::vec4 border_color = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(border_color));
    //         2.3 MINIFYING FILTER - you can start with LINEAR MIPMAP
    //         2.4 MAGNIFYING FILTER - you can start with LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // TASK 3: Generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // TASK 4: Do exactly the same for second texture
    dice_texture = load_texture_2d("images/dice1.png");
    glBindTexture(GL_TEXTURE_2D, dice_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(border_color));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // TASK 11: Come back later :)
    //          create alpha texture as in TASK 4/5
    alpha_texture = load_texture_2d("images/alpha_circle.png");
    glBindTexture(GL_TEXTURE_2D, alpha_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(border_color));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // TASK 5: Get locations of uniforms texture_primary and texture_secondary
    primary_texture_loc = program->get_uniform_location("texture_primary");
    secondary_texture_loc = program->get_uniform_location("texture_secondary");
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

    // Set cameras projection and view matrices and eye location
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
    glm::mat4 view_matrix = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    cube.bind_vao();

    // Model Matrix
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    // Texture
    // TASK 6: For bound object
    //         6.1 set texture_primary uniform to zeroth texture sampler unit
    //         6.2 set zeroth texture sampler unit GL_TEXTURE0 as active
    //         6.3 bind lenna_texture to GL_TEXTURE_2D target
    // TASK 7: Continue to main.frag
    glUniform1i(primary_texture_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lenna_texture);

    // TASK 8: bind dice_texture to texture_secondary
    // TASK 9: Continue to main.frag once again
    glUniform1i(secondary_texture_loc, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dice_texture);

    cube.draw();

    // BLENDING
    // TASK 10: Uncomment this section
    // TASK 11: Go back to init and finish the task
    // TASK 12: Go to main.frag and continue the task
    //teapot.bind_vao();

    // TASK 13: Make the teapot transparent!
    //          13.1 enable blending in the init function
    //          13.2 set blending equation for teapot to be GL_FUNC_ADD (docs.gl -> glBlendEquation)
    //          13.3 set blending factors to be GL_SRC_ALPHA and GL_ONE_MINUS_SRC_ALPHA (docs.gl -> glBlendFunc)
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Model Matrix
    model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    // Texture
    glUniform1i(primary_texture_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, alpha_texture);

    teapot.draw();
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
    }
}

void Application::on_resize(int width, int height) {
    window.resize(width, height);

    // Set the area into which we render
    glViewport(0, 0, width, height);
}
