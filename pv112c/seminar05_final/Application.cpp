#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void Application::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    // Create shader program
    program = make_unique<ShaderProgram>("shaders/main.vert", "shaders/main.frag");

    // Get attribute and uniform locations
    int position_loc = program->get_attribute_location("position");
    int normal_loc = program->get_attribute_location("normal");
    int tex_coord_loc = program->get_attribute_location("texture_coordinate");

    // Get locations of uniforms for positioning and projecting object
    model_matrix_loc = program->get_uniform_location("model_matrix");
    view_matrix_loc = program->get_uniform_location("view_matrix");
    projection_matrix_loc = program->get_uniform_location("projection_matrix");

    // Create geometry
    cube.create_vao(position_loc, normal_loc, tex_coord_loc);

    //----------------------
    //--  Postprocessing  --

    // Create postprocessing shader program
    postprocess_program = make_unique<ShaderProgram>("shaders/postprocess.vert", "shaders/postprocess.frag");

    // Get attribute and uniform locations
    position_loc = postprocess_program->get_attribute_location("position");
    scene_texture_loc = postprocess_program->get_uniform_location("scene_texture");
    app_time_s_loc = postprocess_program->get_uniform_location("app_time_s");

    // Set the sceneTex texture unit to 0
    postprocess_program->use();
    glUniform1i(scene_texture_loc, 0);
    glUseProgram(0);

    // Prepare screen quad geometry and VAO (see first lesson for details)
    glGenBuffers(1, &screen_quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, screen_quad_data, GL_STATIC_DRAW);
    glGenVertexArrays(1, &screen_quad_vao);
    glBindVertexArray(screen_quad_vao);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Prepare FBO textures
    glGenTextures(1, &fbo_color_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window.get_width(), window.get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &fbo_depth_stencil_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_depth_stencil_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window.get_width(), window.get_height(), 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Prepare FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_color_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_stencil_texture, 0);
    GLenum bufs[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, bufs);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw "Failed to create a complete framebuffer";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::render() {
    float app_time_s = float(glfwGetTime());

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, window.get_width(), window.get_height());
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Camera view variables
    glm::mat4 projection_matrix, view_matrix, model_matrix;

    // Note that we calculate aspect ratio from FBO size
    projection_matrix =
        glm::perspective(glm::radians(45.0f), float(window.get_width()) / float(window.get_height()), 0.1f, 100.0f);
    view_matrix = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    program->use();

    // Set cameras projection and view matrices
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    // Render the scene object
    model_matrix = glm::mat4(1.0f);
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    cube.bind_vao();
    cube.draw();

    glBindVertexArray(0);
    glUseProgram(0);

    // Postprocessing

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window.get_width(), window.get_height());
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    postprocess_program->use();
    glUniform1f(app_time_s_loc, app_time_s);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_color_texture);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(screen_quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
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
        this->polygon_mode = GL_LINE;
        break;
    case GLFW_KEY_F:
        this->polygon_mode = GL_FILL;
        break;
    }
}

void Application::on_resize(int width, int height) {
    window.resize(width, height);

    glBindTexture(GL_TEXTURE_2D, fbo_color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, fbo_depth_stencil_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8,
                 nullptr);

    // Set the area into which we render
    glViewport(0, 0, width, height);
}
