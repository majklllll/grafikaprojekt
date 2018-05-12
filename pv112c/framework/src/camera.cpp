#include "camera.hpp"
#include <iostream>

#include <GLFW/glfw3.h>

const float Camera::min_elevation = -1.5f;
const float Camera::max_elevation = 1.5f;
const float Camera::min_distance = 1.0f;
const float Camera::angle_sensitivity = 0.008f;
const float Camera::zoom_sensitivity = 0.003f;

Camera::Camera()
    : angle_direction(0.0f), angle_elevation(0.0f), distance(10.0f), last_x(0), last_y(0), is_rotating(false),
      is_zooming(false) {
  update_eye_pos();
}

void Camera::update_eye_pos() {
  /*eye_position.x = cam.x; //+ distance * cosf(angle_elevation) * -sinf(angle_direction);
  eye_position.y = cam.y; //+ distance * sinf(angle_elevation);
  eye_position.z = cam.z; //+ distance * cosf(angle_elevation) * cosf(angle_direction);
  */

    eye_position.x = cam.x; //+ /*distance *  cosf(angle_elevation) * -sinf(angle_direction);
    eye_position.y = cam.y; //+ /*distance *  sinf(angle_elevation);
    eye_position.z = cam.z; //+ /*distance *  cosf(angle_elevation) * cosf(angle_direction);

  std::cout << eye_position.x << " - " << eye_position.y << " - " <<eye_position.z << std::endl;
}

void Camera::on_mouse_button(int button, int action, int mods) {
  // Left mouse button affects the angles
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      is_rotating = true;
    } else {
      is_rotating = false;
    }
  }
  // Right mouse button affects the zoom
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      is_zooming = true;
    } else {
      is_zooming = false;
    }
  }
}

void Camera::on_mouse_move(double x, double y) {
  float dx = float(x - last_x);
  float dy = float(y - last_y);
  last_x = static_cast<int>(x);
  last_y = static_cast<int>(y);

  if (is_rotating) {
    angle_direction += dx * angle_sensitivity;
    angle_elevation += dy * angle_sensitivity;

    // Clamp the results
    if (angle_elevation > max_elevation)
      angle_elevation = max_elevation;
    if (angle_elevation < min_elevation)
      angle_elevation = min_elevation;
  }
  if (is_zooming) {
    distance *= (1.0f + dy * zoom_sensitivity);

    // Clamp the results
    if (distance < min_distance)
      distance = min_distance;
  }
  update_eye_pos();
}

glm::vec3 Camera::get_eye_position() const { return eye_position; }
glm::vec3 Camera::get_center_of_view() const {return glm::vec3(cam.x - 10.0f, cam.y, cam.z); }
