#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class SphereModel
{
private:
    static std::vector<GLfloat> vertices;
    static std::vector<GLuint> indices;

public:
    static void init(int limit);
    static const std::vector<GLfloat>& getVertices();
    static const std::vector<GLuint>& getIndices();
};