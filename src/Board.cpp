#include "Board.hpp"

void Board::init()
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

    // bubbles
    glGenBuffers(1, &bubble_buffer);

    // uniforms
    vp_loc = glGetUniformLocation(shader, "vp");
    camera_loc = glGetUniformLocation(shader, "cameraPos");

    loadLevel(1);

    // walls
    wall.init(min_x, max_x,
              min_y, max_y,
              min_z, max_z);

    noise.SetNoiseType(FastNoise::SimplexFractal);
}

void Board::update()
{
    static float step = 2.0f;

    static double last_time = glfwGetTime();
    double current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    float noise_off = float(current_time);
    float delta = 1.0f;

    for (Bubble& b : bubbles)
    {
        auto& offset = b.position;
        auto& scale = b.scale;

        float speed = b.special ? 1.0f : bubble_speed;

        float p1 = noise.GetSimplexFractal(noise_off, 0) * noise_sens;
        float p2 = noise.GetSimplexFractal(0, noise_off) * noise_sens;

        float x = offset.x + p1 * delta_time;
        float y = offset.y + scale * speed * delta_time;
        float z = offset.z + p2 * delta_time;

        if (std::abs(x) <= max_x - delta && z > delta && z < max_z - delta)
        {
            offset.x = x;
            offset.z = z;
        }

        if (y >= max_y + delta)
        {
            if (!b.special)
                y = min_y - delta;
        }
        offset.y = y;

        noise_off += step;
    }

    glBindBuffer(GL_ARRAY_BUFFER, bubble_buffer);
    glBufferData(GL_ARRAY_BUFFER, bubbles.size() * sizeof(Bubble), &bubbles[0], GL_DYNAMIC_DRAW);
}

void Board::draw()
{
    wall.draw(bubbles);

    glBindVertexArray(vao);
    glUseProgram(shader);

    glm::mat4 vp = Camera::getVP();
    glUniformMatrix4fv(vp_loc, 1, GL_FALSE, &vp[0][0]);

    glm::vec3 camera_pos = Camera::getPosition();
    glUniform3f(camera_loc, camera_pos.x, camera_pos.y, camera_pos.z);

    glUniform3f(glGetUniformLocation(shader, "dirLight.dir"), 1.0f, 1.0f, -1.0f);
        
    int idx = 0;

    for (auto& b : bubbles)
        if (b.special)
        {
            std::string loc = "pointLights[" + std::to_string(idx++) + "]";
            glUniform3f(glGetUniformLocation(shader, (loc + ".pos").c_str()), b.position.x, b.position.y, b.position.z);
        }

    glUniform1i(glGetUniformLocation(shader, "pointLightCount"), idx);

    glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0, bubbles.size());
    glBindVertexArray(0);
}

bool Board::collided(const glm::vec3& position, const GLfloat& radius, int& score_add)
{
    score_add = 0;

    for (auto& b : bubbles)
        if (glm::dot(b.position - position, b.position - position)
                     <= (radius + b.scale) * (radius + b.scale))
        {
            if (b.special)
            {
                b.position.z = -100.0f;
                ++score_add;
            }
            else
                return true;
        }

    return false;
}

void Board::loadLevel(GLuint level)
{
    bubble_speed = 0.3f + level * 0.3f;
    std::cout << "Loading LEVEL " << level << "\n";
    restart(level * 25);
}

void Board::restart(size_t size)
{
    if (size > 0)
    {
        bubbles.clear();
        bubbles.resize(size);
    }

    size = bubbles.size();

    for (size_t i = 0; i < size; ++i)
    {
        bool special = i < std::min(5, int(size) / 20);
        if (special)
            createSpecial(bubbles[i]);
        else
            createNormal(bubbles[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, bubble_buffer);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(Bubble), &bubbles[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)0);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)(sizeof(glm::vec3)));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bubble), (void*)(sizeof(glm::vec3) + sizeof(GLfloat)));
    glVertexAttribDivisor(3, 1);
}

void Board::createNormal(Bubble& b)
{
    GLfloat scale = Random::random(0.2f, 0.7f);

    GLfloat x = Random::random(min_x, max_x);
    GLfloat y = Random::random(min_y, max_y);
    GLfloat z = Random::random(0.0f, max_z);
    glm::vec3 position(x, y, z);

    float rand = Random::random<float>();
    Color color;

    if (rand < 0.5f)
        color = Color::PINK;
    else
        color = Color::LIGHT_BLUE;

    color.a = 0.5f;

    b = {position, scale, color, 0.0f, false};
}

void Board::createSpecial(Bubble& b)
{
    GLfloat scale = Random::random(0.1f, 0.6f);

    GLfloat x = Random::random(min_x, max_x);
    GLfloat y = Random::random(min_y, min_y + 1);
    GLfloat z = Random::random(0.0f, max_z);
    glm::vec3 position(x, y, z);

    Color color = Color::GREEN;
    color.a = 0.5f;

    b = {position, scale, color, 0.0f, true};
}

void Board::terminate()
{
    wall.terminate();

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &bubble_buffer);
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
}
