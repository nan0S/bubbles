#include "Camera.hpp"

glm::mat4 Camera::projection = glm::mat4();
glm::mat4 Camera::view = glm::mat4();

glm::vec3 Camera::position = glm::vec3(0, 0, -2);

float Camera::fov = 60.0f;

void Camera::init()
{
    glfwSetScrollCallback(Window::get(), scrollHandler);
}

void Camera::compute(glm::vec3 position, glm::vec3 direction, glm::vec3 right, glm::vec3 up)
{
    projection = glm::perspective(glm::radians(fov), Window::getAspectRatio(), 0.1f, 100.0f);
    view = glm::lookAt(
        position,
        position + direction,
        up
    );

    Camera::position = position;
}

glm::mat4 Camera::getVP()
{
    return projection * view;
}

const glm::mat4& Camera::getView()
{
    return view;
}

const glm::vec3& Camera::getPosition()
{
    return position;
}

void Camera::scrollHandler(GLFWwindow* window, double xoffset, double yoffset)
{
    fov = glm::clamp(fov - float(yoffset), 10.0f, 89.0f);
}