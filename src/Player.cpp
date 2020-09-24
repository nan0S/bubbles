#include "Player.hpp"

void Player::init()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    shader = LoadShaders("../shader/bubbleVertex.glsl", "../shader/bubbleFragment.glsl");
    
    // vertices
    const std::vector<GLfloat>& vertices = SphereModel::getVertices();
    vertex_count = vertices.size();

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

    // indices
    const std::vector<GLuint>& indices = SphereModel::getIndices();
    index_count = indices.size();
    
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // uniforms
    vp_loc = glGetUniformLocation(shader, "vp");
    camera_loc = glGetUniformLocation(shader, "cameraPos");

    // bubble
    glGenBuffers(1, &bubble_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, bubble_buffer);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)0);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)(sizeof(glm::vec3)));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)(sizeof(glm::vec3) + sizeof(GLfloat)));
    glVertexAttribDivisor(3, 1);

    restart();
}       

void Player::update(GLfloat min_x, GLfloat max_x, GLfloat min_y, GLfloat max_y, GLfloat min_z, GLfloat max_z)
{   
    static bool last_state = Window::pressed(GLFW_KEY_TAB);
    bool current_state = Window::pressed(GLFW_KEY_TAB);
    if (current_state && !last_state)
        locked = !locked;
    last_state = current_state;

    static double last_time = glfwGetTime();
    double current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    double xpos, ypos;
    glfwGetCursorPos(Window::get(), &xpos, &ypos);
    
    horizontal_angle += mouse_speed * float(last_xpos - xpos);
    vertical_angle += mouse_speed * float(last_ypos - ypos);
    vertical_angle = glm::clamp(vertical_angle, (float)-M_PI_2, (float)M_PI_2);

    last_xpos = xpos;
    last_ypos = ypos;

    direction = glm::vec3(
        cos(vertical_angle) * sin(horizontal_angle),
        sin(vertical_angle),
        cos(vertical_angle) * cos(horizontal_angle)
    );

    right = glm::vec3(
        sin(horizontal_angle - M_PI_2),
        0,
        cos(horizontal_angle - M_PI_2)
    );

    up = glm::cross(right, direction);

    glm::vec3 delta_pos(0.0f);

    if (Window::pressed(GLFW_KEY_W))
        delta_pos += direction;
    if (Window::pressed(GLFW_KEY_S))
        delta_pos -= direction;
    if (Window::pressed(GLFW_KEY_D))
        delta_pos += right;
    if (Window::pressed(GLFW_KEY_A))
        delta_pos -= right;
    if (Window::pressed(GLFW_KEY_SPACE))
        delta_pos += up;
    if (Window::pressed(GLFW_KEY_LEFT_ALT))
        delta_pos -= up;

    delta_pos *= delta_time * speed;

    auto& offset = player.position;
    offset += delta_pos;

    float delta = 0.3f;
    offset.x = glm::clamp(offset.x, min_x + delta, max_x - delta);
    offset.y = glm::clamp(offset.y, min_y + delta, max_y - delta);
    offset.z = glm::clamp(offset.z, min_z + delta, max_z);
    
    glBindBuffer(GL_ARRAY_BUFFER, bubble_buffer);
    glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(Bubble), &player, GL_DYNAMIC_DRAW);

    if (locked)
        Camera::compute(
            glm::vec3(-10.0f, 0.0f, glm::clamp(offset.z, min_z + 3.1f, max_z - 3.1f)),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    else
        Camera::compute(offset, direction, right, up);
}

void Player::draw()
{
    glBindVertexArray(vao);
    glUseProgram(shader);   

    // uniforms
    glm::mat4 vp = Camera::getVP();
    glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &vp[0][0]);

    glm::vec3 camera_pos = Camera::getPosition();
    glUniform3f(camera_loc, camera_pos.x, camera_pos.y, camera_pos.z);

    glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0, 1);
    glBindVertexArray(0);
}

const glm::vec3& Player::getPosition()
{
    return player.position;
}

void Player::restart()
{
    player.position = glm::vec3(0, 0, -2);
    player.scale = 0.5f;
    player.color = Color::WHITE;
    player.color.a = 1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, bubble_buffer);
    glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(Bubble), &player, GL_DYNAMIC_DRAW);

    direction = glm::vec3(0, 0, 1);
    right = glm::vec3(-1, 0, 0);
    up = glm::vec3(0, 1, 0);

    horizontal_angle = 0;
    vertical_angle = 0;   
    
    last_xpos = 0;
    last_ypos = 0;

    glfwGetCursorPos(Window::get(), &last_xpos, &last_ypos);
}

const GLfloat& Player::getRadius()
{
    return player.scale;
}

void Player::terminate()
{
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &bubble_buffer);
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
}