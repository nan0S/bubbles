#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "SphereModel.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Common.hpp"

class Player
{
private:
    GLuint vao;
    GLuint shader;

    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint bubble_buffer;

    GLuint vp_loc;
    GLuint camera_loc;
    GLuint light_loc;

    size_t vertex_count;
    size_t index_count;

    Bubble player;

    glm::vec3 direction = glm::vec3(0, 0, 1);
    glm::vec3 right = glm::vec3(-1, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);

    float horizontal_angle = 0;
    float vertical_angle = 0;   
    float speed = 3.0f;
    float mouse_speed = 0.005f;
    
    double last_xpos = 0;
    double last_ypos = 0;

    bool locked = false;

public:
    void init();
    void update(GLfloat min_x, GLfloat max_x,
                GLfloat min_y, GLfloat max_y,
                GLfloat min_z, GLfloat max_z);
    void draw();
    void restart();
    
    const glm::vec3& getPosition();
    const GLfloat& getRadius();

    void terminate();
};