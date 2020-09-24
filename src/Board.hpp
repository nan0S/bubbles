#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>

#include "Common.hpp"
#include "Shader.hpp"
#include "SphereModel.hpp"
#include "Camera.hpp"
#include "Wall.hpp"
#include "FastNoise.h"

class Board
{
private:
    GLuint vao;
    GLuint shader;

    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint bubble_buffer;

    GLuint vp_loc;
    GLuint camera_loc;

    size_t vertex_count;
    size_t index_count;

    std::vector<Bubble> bubbles;

    Wall wall;
    FastNoise noise;

    float bubble_speed = 1.0f;
    float noise_sens = 1.0f;

public:
    GLfloat min_x = -5.0f, max_x = 5.0f;
    GLfloat min_z = -3.0f, max_z = 30.0f;
    GLfloat min_y = -5.0f, max_y = 5.0f; 

    void init();
    void update();
    bool collided(const glm::vec3& position, const GLfloat& radius, int& score_add);
    void draw();
    void loadLevel(GLuint level);
    void restart(size_t new_size = 0);
    void terminate();

    void createNormal(Bubble& b);
    void createSpecial(Bubble& b);
};