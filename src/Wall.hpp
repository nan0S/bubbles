#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Common.hpp"

class Wall
{
private:
    GLuint vao;
    GLuint shader;

    GLuint vertex_buffer;
    GLuint normal_buffer;
    GLuint index_buffer;

    GLuint camera_loc;
    GLuint mvp_loc;

public:
    void init(GLfloat min_x, GLfloat max_x,
              GLfloat min_y, GLfloat max_y,
              GLfloat min_z, GLfloat max_z);
    void draw(std::vector<Bubble>& bubbles);
    void terminate();
};