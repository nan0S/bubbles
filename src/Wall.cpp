#include "Wall.hpp"

void Wall::init(GLfloat min_x, GLfloat max_x, GLfloat min_y, GLfloat max_y, GLfloat min_z, GLfloat max_z)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    shader = LoadShaders("../shader/wallVertex.glsl", "../shader/wallFragment.glsl");

    static const GLfloat vertices[] = {
        max_x, min_y, min_z,
        max_x, min_y, max_z, 
        max_x, max_y, max_z,
        max_x, max_y, min_z, 

        min_x, min_y, min_z, 
        min_x, max_y, min_z, 
        min_x, max_y, max_z, 
        min_x, min_y, max_z, 

        max_x, min_y, min_z, 
        max_x, max_y, min_z, 
        min_x, max_y, min_z, 
        min_x, min_y, min_z, 

        max_x, min_y, max_z, 
        min_x, min_y, max_z, 
        min_x, max_y, max_z, 
        max_x, max_y, max_z, 

        max_x, min_y, min_z, 
        min_x, min_y, min_z, 
        min_x, min_y, max_z, 
        max_x, min_y, max_z, 

        max_x, max_y, min_z, 
        max_x, max_y, max_z, 
        min_x, max_y, max_z, 
        min_x, max_y, min_z
    };

    static const GLfloat normals[] = {
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };  

    static const GLuint indices[] = {
        0, 1, 2,
        0, 2, 3, 

        4, 5, 6, 
        4, 6, 7,

        8, 9, 10, 
        8, 10, 11, 

        12, 13, 14, 
        12, 14, 15, 

        16, 17, 18, 
        16, 18, 19, 

        20, 21, 22, 
        20, 22, 23
    };

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    camera_loc = glGetUniformLocation(shader, "cameraPos");
    mvp_loc = glGetUniformLocation(shader, "mvp");
}

void Wall::draw(std::vector<Bubble>& bubbles)
{
    glBindVertexArray(vao);
    glUseProgram(shader);

    glm::vec3 camera_pos = Camera::getPosition();
    glUniform3f(camera_loc, camera_pos.x, camera_pos.y, camera_pos.z);

    glm::mat4 mvp = Camera::getVP();
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

    int idx = 0;

    for (auto& b : bubbles)
        if (b.special)
        {
            std::string loc = "pointLights[" + std::to_string(idx++) + "]";
            glUniform3f(glGetUniformLocation(shader, (loc + ".pos").c_str()), b.position.x, b.position.y, b.position.z);
        }

    glUniform1i(glGetUniformLocation(shader, "pointLightCount"), idx);
    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
}

void Wall::terminate()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
}