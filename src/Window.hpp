#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

class Window
{
public:
    static GLFWwindow *window;
    static int width, height;

    static bool create(int width, int height, std::string title);

    static GLFWwindow* get();
    static int getWidth();
    static int getHeight();
    static float getAspectRatio();

    static void resizeHandler(GLFWwindow* window, int w, int h);
    static bool pressed(GLuint key);
    static bool shouldClose();
    static void clear();
    static void display();
};