#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

class Camera
{
private:
    static glm::mat4 projection;
    static glm::mat4 view;

    static glm::vec3 position;

    static float fov;

public:
    static void init();
    static void compute(glm::vec3 position, 
                        glm::vec3 direction, 
                        glm::vec3 right, 
                        glm::vec3 up);
    
    static glm::mat4 getVP();
    static const glm::mat4& getView();
    static const glm::vec3& getPosition();

    static void scrollHandler(GLFWwindow* window, double xoffset, double yoffset);
};